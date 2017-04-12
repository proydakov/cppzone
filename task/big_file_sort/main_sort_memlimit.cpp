#include <map>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

size_t g_allocated_total = 0;
size_t obj_size = 0;
size_t allocated_size = 0;

void* operator new(size_t sz) throw (std::bad_alloc)
{
    /*
    if(sz != obj_size) {
        obj_size = sz;
        std::cout << "size: " << sz << std::endl;
    }
    */

    void* mem = malloc(sz);
    if (nullptr == mem) {
        throw std::bad_alloc();
    }

#ifdef _MSC_VER 
    const size_t allocated_step = sz;
#else
    size_t* ptr = (size_t*)(mem);
    ptr -= 1;
    const size_t allocated_step = *ptr;
#endif

    g_allocated_total += allocated_step;

    /*
    if(allocated_step != allocated_size) {
        allocated_size = allocated_step;
        std::cout << "allocated_size: " << allocated_size << std::endl;
    }
    */

    return mem;
}

void operator delete(void* ptr) throw()
{
    free(ptr);
}

struct data
{
    std::string email;
    size_t      count;

    friend bool operator<(const data& obj1, const data& obj2)
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

template<class container>
std::string sort_blob(container& vec, const std::string& opath, size_t blob_index)
{
    std::stringstream sstream;
    sstream << opath << ".blob" << blob_index;

    const std::string blob_path(sstream.str());
    std::cout << "sort: " + blob_path << std::endl;

    std::sort(vec.begin(), vec.end());

    std::cout << "dump: " + blob_path << std::endl;
    std::ofstream output(blob_path);
    std::for_each(vec.begin(), vec.end(), [&output](const data& obj) {
        write_data(output, obj);
    });

    return blob_path;
}

void merge_blob(const std::vector<std::string>& blobs, const std::string& opath)
{
    std::multimap<data, size_t> sources;
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
        if( !input.eof( ) ) {
            sources.insert( std::make_pair( buffer, it->second ) );
        }
        sources.erase(it);
    }
}

class csource
{
public:
    csource(const std::string& ifile, size_t limit) : input_(ifile), limit_(limit)
    {
    }

    std::deque<data> read()
    {
        std::deque<data> vec;

        std::lock_guard<std::mutex> guard(mutex_);

        data buffer;
        g_allocated_total = 0;
        while (true) {
            read_data(input_, buffer);

            if( input_.eof( ) ) {
                break;
            }

            vec.push_back( std::move(buffer) );

            if(g_allocated_total > limit_) {
                break;
            }
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

    const size_t hardware_concurrency = std::thread::hardware_concurrency();

    const size_t GB_SIZE = 1024 * 1024 * 1024;
    std::string::size_type sz;
    double limit_gb = std::stod(ssize, &sz);
    double k_safe = 0.975;
    const size_t MEMORY_LIMIT = k_safe * limit_gb * GB_SIZE / hardware_concurrency;

    size_t blob_counter = 0;
    cblobs blobs;
    csource source(ifile, MEMORY_LIMIT);

    std::vector<std::thread> threads;
    for(size_t i = 0; i < hardware_concurrency; i++) {
        threads.push_back(std::move(std::thread([&source, &blobs, &ifile, &blob_counter]() {
            while(true) {
                std::deque<data> vec = source.read();

                if(vec.empty()) {
                    break;
                }

                size_t counter = blob_counter++;
                auto str = sort_blob(vec, ifile, counter);
                blobs.add_blob( std::move(str) );
            }
        })));
    }

    for(size_t i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    auto vec = blobs.get_blobs();
    merge_blob(vec, ofile);

    for(size_t i = 0; i < vec.size(); i++) {
        std::remove(vec[i].c_str());
    }

    return 0;
}
