#include <string>
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

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " path/to/input/file path/to/output/file" << std::endl;
        return 1;
    }

    std::string ifile(argv[1]);
    std::string ofile(argv[2]);

    std::vector<data> vec;

    data buffer;
    std::ifstream input(ifile);
    while (true) {
        read_data(input, buffer);
        if( input.eof( ) ) break;
        vec.push_back( buffer );
    }

    std::cout << "size: " << vec.size() << std::endl;

    std::sort(vec.begin(), vec.end(), [](const data& obj1, const data& obj2){
        return obj1.email < obj2.email;
    });

    std::ofstream output(ofile);
    for(size_t i = 0; i < vec.size(); i++) {
        write_data(output, vec[i]);
    }

    return 0;
}
