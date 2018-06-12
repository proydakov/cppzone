#include <string>
#include <chrono>
#include <vector>
#include <cstdint>
#include <iostream>

std::int64_t constexpr giga = 1024l * 1024l * 1024l;

template<class T>
void print_stat(T start, T end, std::size_t bytes)
{
    auto const delta = end - start;
    std::cout << "fill size: " << bytes * 1.0 / giga << " Gb time: " << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() << " milliseconds" << std::endl;
    std::cout << "memory_bandwidth: " << bytes * 1.0 / giga / std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() * 1000 << " GB/s" << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 0;
    }

    std::int64_t const gb = std::stol(argv[1]);
    std::cout << "memory size: " << gb << " Gb" << std::endl;

    using vec_t = std::vector<char>;
    vec_t data;

    std::int64_t const alloc_size = gb * giga;

    {
        std::cout << "\nallocation..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        data.resize(alloc_size, 0);
        auto end = std::chrono::high_resolution_clock::now();

        print_stat(start, end, data.size());
    }

    vec_t copy;

    {
        std::cout << "\ncopying..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        copy = data;
        auto end = std::chrono::high_resolution_clock::now();

        print_stat(start, end, data.size());
    }

    {
        std::cout << "\nwaiting..." << std::endl;
        std::int32_t n{ 0 };
        std::cin >> n;
    }

    return 0;
}
