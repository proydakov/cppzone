#include <chrono>
#include <vector>
#include <cstdint>
#include <iostream>

int main()
{
    std::int64_t constexpr giga = 1024l * 1024l * 1024l;

    using vec_t = std::vector<std::int64_t>;
    vec_t data;

    std::int64_t const alloc_size = 4l * giga;
    std::cout << "alloc_size: " << alloc_size / giga << " Gb" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    data.resize(alloc_size, 0);
    auto end = std::chrono::high_resolution_clock::now();

    auto delta = end - start;
    std::cout << "fill size: " << data.size() * 1.0 / giga << " Gb time: " << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() << " milliseconds" << std::endl;
    std::cout << "memory_bandwidth: " << data.size() * 1.0 / giga / std::chrono::duration_cast<std::chrono::seconds>(delta).count() << " GB/s" << std::endl;

    std::int32_t n{ 0 };
    std::cin >> n;

    return 0;
}
