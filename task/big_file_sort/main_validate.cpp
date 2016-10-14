#include <fstream>
#include <sstream>
#include <iostream>

struct data
{
    std::string email;
    size_t      count;
};

void parse(const std::string& str, data& obj)
{
    std::stringstream sstream;
    sstream << str;
    sstream >> obj.email;
    sstream >> obj.count;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " path/to/file" << std::endl;
        return 1;
    }

    std::string file(argv[1]);

    data obj1;

    std::ifstream input(file);
    std::string line;

    std::getline(input, line);
    parse(line, obj1);

    data obj2;

    while (std::getline(input, line)) {
        parse(line, obj2);
        if(obj2.email < obj1.email) {
            std::cerr << "file: " << file << " is unsorted" << std::endl;
            return 1;
        }
        obj1 = obj2;
    }
    std::cout << "file: " << file << " is sorted" << std::endl;

    return 0;
}
