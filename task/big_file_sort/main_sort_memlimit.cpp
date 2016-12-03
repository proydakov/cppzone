#include <map>
#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

size_t allocated_total = 0;
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

    allocated_total += allocated_step;

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
    std::cout << "sort: " << blob_path << std::endl;

    std::sort(vec.begin(), vec.end());

    std::cout << "dump: " << blob_path << std::endl;
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
        std::ifstream input(blobs[i]);
        read_data(input, buffer);

        inputs.push_back(std::move(input));
        sources.insert( std::make_pair( buffer, i ) );
    }
    std::cout << "merge: " << opath << std::endl;

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

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout << "usage: " << argv[0] << " path/to/input/file path/to/output/file memlimit (GB)" << std::endl;
        return 1;
    }

    const std::string ifile(argv[1]);
    const std::string ofile(argv[2]);
    const std::string ssize(argv[3]);

    data buffer;
    std::ifstream input(ifile);
    if(!input.is_open()) {
        std::cout << "file not found: " << argv[0] << std::endl;
        return 1;
    }

    const size_t GB_SIZE = 1024 * 1024 * 1024;

    std::string::size_type sz;
    double limit_gb = std::stod(ssize, &sz);
    const size_t MEMORY_LIMIT = 0.975 * limit_gb * GB_SIZE;

    std::vector<std::string> blobs;
    {
        size_t blob_counter = 0;

        std::deque<data> vec;
        allocated_total = 0;
        while (true) {
            read_data(input, buffer);

            if( input.eof( ) ) break;
            vec.push_back( std::move(buffer) );

            if(allocated_total > MEMORY_LIMIT) {
                auto str = sort_blob(vec, ifile, blob_counter);
                blob_counter++;
                blobs.push_back( std::move(str) );
                vec.resize(0);
                vec.shrink_to_fit();
                allocated_total = 0;
            }
        }
        auto str = sort_blob(vec, ifile, blob_counter);
        blobs.push_back( std::move(str) );
    }

    merge_blob(blobs, ofile);

    return 0;
}
