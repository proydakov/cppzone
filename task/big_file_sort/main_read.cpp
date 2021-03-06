#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " <path/to/file>" << std::endl;
        return 1;
    }

    const std::string file(argv[1]);
    std::ifstream input(file);

    long counter = 0;
    enum { buffer_size = 1024 * 1024 };
    std::vector<char> buffer(buffer_size, 0);
    while(!input.eof( )) {
        input.read(buffer.data(), buffer_size);
        const auto gcount = input.gcount();
        counter += static_cast<long>(std::count( buffer.data(), buffer.data() + gcount, '\n' ));
    }
    std::cout << "file_size: " << counter << std::endl;

    return 0;
}
