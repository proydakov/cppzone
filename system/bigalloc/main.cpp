#include <vector>
#include <iostream>

int main(int, char*[])
{
    size_t s = 1l * 1024 * 1024 * 1024 / sizeof(int) * 4;
    std::vector<int> blob( s, 0 );

    size_t i = 0;
    std::cout << "wait input..." << std::endl;
    std::cin >> i;

    return 0;
}
