#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <atomic>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

struct data
{
    std::string email;
    size_t      count;
};

struct comparator
{
    bool operator()(const data& obj1, const data& obj2) const
    {
        return obj1.email < obj2.email;
    }
};

template<class cstream>
void read_data(cstream& stream, data& obj)
{
    stream >> obj.email;
    stream >> obj.count;
}

template<class cstream>
void write_data(cstream& stream, const data& obj)
{
    stream << obj.email;
    stream << " ";
    stream << obj.count;
    stream << "\n";
}

template<class container, class compare>
std::string sort_blob(container& vec, const std::string& opath, size_t blob_index, compare comp)
{
    std::string blob_path;
    blob_path.append(opath);
    blob_path.append(".blob");
    blob_path.append(std::to_string(blob_index));

    std::cout << "sort: " + blob_path << std::endl;

    std::sort(vec.begin(), vec.end(), comp);

    std::cout << "dump: " + blob_path << std::endl;
    std::ofstream output(blob_path);
    std::for_each(vec.begin(), vec.end(), [&output](const data& obj) {
        write_data(output, obj);
    });

    return blob_path;
}

void merge_blob(const std::vector<std::string>& blobs, const std::string& opath)
{
    std::multimap<data, size_t, comparator> sources;
    std::vector<std::ifstream> inputs;

    data buffer;
    for(size_t i = 0; i < blobs.size(); i++) {
        inputs.emplace_back(blobs[i].c_str());
        read_data(inputs.back(), buffer);
        sources.insert( std::make_pair( buffer, i ) );
    }
    std::cout << "merge: " + opath << std::endl;

    std::ofstream output(opath);
    while(!sources.empty()) {
        auto it = sources.begin();
        write_data(output, it->first);
        auto& input = inputs[it->second];
        read_data(input, buffer);
        if( !input.eof() ) {
            sources.insert( std::make_pair( buffer, it->second ) );
        }
        sources.erase(it);
    }
}

class csource
{
public:
    csource(const std::string& ifile, size_t limit)
        : input_(ifile)
        , limit_(limit)
    {
    }

    std::vector<data> read_batch()
    {
        data buffer;
        size_t bytes{};
        std::vector<data> vec;

        std::lock_guard<std::mutex> guard(mutex_);
        while (bytes <= limit_) {
            read_data(input_, buffer);

            if( input_.eof() ) {
                break;
            }

            bytes += sizeof(data);
            // This is hint to calculate real memory usage
            if (buffer.email.capacity() > sizeof(buffer.email)) {
                bytes += buffer.email.capacity();
            }

            vec.emplace_back( std::move(buffer) );
        }
        return vec;
    }

private:
    std::mutex mutex_;
    std::ifstream input_;
    const size_t limit_;
};

class cblobs
{
public:
    ~cblobs()
    {
        for(auto const& blob : blobs_) {
            std::remove(blob.c_str());
        }
    }

    void add_blob(const std::string& blob)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        blobs_.push_back(blob);
    }

    std::vector<std::string> get_blobs()
    {
        std::lock_guard<std::mutex> guard(mutex_);
        return blobs_;
    }

private:
    std::mutex mutex_;
    std::vector<std::string> blobs_;
};

double get_limit_gb(const std::string& limit)
{
    std::string::size_type sz;
    return std::stod(limit, &sz);
}

void worker(csource& source, cblobs& blobs, const std::string& ifile, std::atomic<size_t>& blob_counter)
{
    while(true) {
        auto vec = source.read_batch();

        if(vec.empty()) {
            break;
        }

        auto const next_counter = blob_counter++;
        auto blob_path = sort_blob(vec, ifile, next_counter, comparator());
        blobs.add_blob( std::move(blob_path) );
    }
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " <path/input/file> <path/output/file> <memlimit (GB)>" << std::endl;
        return 1;
    }

    const std::string ifile(argv[1]);
    const std::string ofile(argv[2]);
    const std::string ssize(argv[3]);

    {
        std::ifstream input(ifile);
        if(!input.is_open()) {
            std::cout << "file not found: " << argv[0] << std::endl;
            return 1;
        }
    }

    constexpr size_t GB_SIZE = 1024 * 1024 * 1024;
    const double limit_gb = get_limit_gb(ssize);
    const size_t hardware_concurrency = std::thread::hardware_concurrency();
    const size_t memory_limit = static_cast<size_t>(limit_gb * double(GB_SIZE) / double(hardware_concurrency) * 0.975);

    csource source(ifile, memory_limit);
    cblobs blobs;
    std::atomic<size_t> blob_counter = 0;

    std::vector<std::thread> threads;
    for(size_t i = 0; i < hardware_concurrency; i++) {
        threads.emplace_back(worker, std::ref(source), std::ref(blobs), std::ref(ifile), std::ref(blob_counter));
    }

    for(auto& thread : threads) {
        thread.join();
    }

    merge_blob(blobs.get_blobs(), ofile);

    return 0;
}
