#include <bitset>
#include <string>
#include <cstdint>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "usage: app <num>\n";
        return 1;
    }

    auto const num = std::stoul(argv[1]);
    if (num > sizeof(std::uint64_t) * 8)
    {
        std::cout << "too big input num\n";
        return 1;
    }

    std::uint64_t mask{num == 0 ? 0ul : ~0ul};
    mask = mask >> (sizeof(std::uint64_t) * 8 - num);

    std::cout << "num: " << num << " mask: " << std::bitset<sizeof(std::uint64_t) * 8>(mask) << "\n";

    return 0;
}
