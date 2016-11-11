#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

struct data
{
    std::string email;
    size_t      count;
};

template<class cstream>
void read_data(cstream& stream, data& obj)
{
    stream >> obj.email;
    stream >> obj.count;
}

template<class cstream>
void write_data(cstream& stream, data& obj)
{
    stream << obj.email;
    stream << " ";
    stream << obj.count;
    stream << "\n";
}

size_t GB_SIZE = 1024 * 1024 * 1024;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " path/to/input/file path/to/output/file" << std::endl;
        return 1;
    }

    std::string ifile(argv[1]);
    std::string ofile(argv[2]);

    std::vector<data> vec;

    data buffer;
    std::ifstream input(ifile);

    size_t allocated = 0;

    while (true) {
        read_data(input, buffer);

        if( input.eof( ) ) break;
        vec.push_back( std::move( buffer ) );
        allocated += vec.back().email.capacity() * sizeof(char);

        if((vec.size() == vec.capacity()) && ((vec.capacity() * 2 * sizeof(data) + allocated) > GB_SIZE)) {
            break;
        }
    }

    std::cout << "allocated: " << allocated << std::endl;
    std::cout << "data size: " << vec.size() << std::endl;
    std::cout << "data cap:  " << sizeof(data) * vec.capacity() << std::endl;

    int i = 0;
    std::cin >> i;

    return 0;
}
