#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

struct data
{
    std::string email;
    size_t      count;
};

template<class cstream>
void parse(cstream& sstream, data& obj)
{
    sstream >> obj.email;
    sstream >> obj.count;
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " path/to/file" << std::endl;
        return 1;
    }

    std::string file(argv[1]);

    data obj1;

    std::ifstream input(file);
    parse(input, obj1);

    data obj2;
    while (true) {
        parse(input, obj2);
        if(input.eof()) break;
        if(obj2.email < obj1.email) {
            std::cerr << "file: " << file << " is unsorted" << std::endl;
            return 1;
        }
        obj1 = obj2;
    }
    std::cout << "file: " << file << " is sorted" << std::endl;

    return 0;
}
