#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <iostream>

std::size_t constexpr giga = 1024ul * 1024ul * 1024ul;

class experiment
{
public:
    experiment(std::size_t alloc_bytes)
        : m_bytes(alloc_bytes)
        , m_start(std::chrono::high_resolution_clock::now())
    {
    }

    ~experiment()
    {
        auto const end = std::chrono::high_resolution_clock::now();

        auto const delta = end - m_start;
        std::cout << "memory_bandwidth: " << std::setprecision(3) << double(m_bytes) / double(giga) / double(std::chrono::duration_cast<std::chrono::milliseconds>(delta).count()) * double(1000) << " GiB/s" << std::endl;
    }

private:
    std::size_t const m_bytes;
    std::chrono::time_point<std::chrono::high_resolution_clock> const m_start;
};


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 0;
    }

    std::size_t const gb = std::stoul(argv[1]);
    std::cout << "memory size: " << gb << " Gb" << std::endl;

    using vec_t = std::vector<std::int64_t>;
    vec_t origin;

    std::size_t const alloc_bytes = gb * giga;
    std::size_t const alloc_size = alloc_bytes / sizeof(vec_t::value_type);

    {
        std::cout << "\nallocating..." << std::endl;
        experiment e(alloc_bytes);
        origin.resize(alloc_size, 1);
    }

    {
        std::cout << "\nmemset..." << std::endl;
        experiment e(alloc_bytes);
        std::memset(origin.data(), 7, alloc_bytes);
    }

    {
        std::cout << "\nreading..." << std::endl;
        experiment e(alloc_bytes);
        auto end_ptr = (volatile int64_t*)((char*)origin.data() + alloc_bytes);
        for (auto ptr = (volatile int64_t*)(origin.data()); ptr < end_ptr; ptr++)
        {
            [[maybe_unused]] volatile int64_t val = *ptr;
        }
    }

   {
        std::cout << "\nwriting..." << std::endl;
        experiment e(alloc_bytes);
        auto end_ptr = (volatile int64_t*)((char*)origin.data() + alloc_bytes);
        for (auto ptr = (volatile int64_t*)(origin.data()); ptr < end_ptr; ptr++)
        {
            *ptr = 999;
        }
    }

   {
        std::cout << "\nreading + writing..." << std::endl;
        experiment e(alloc_bytes * 2);
        auto end_ptr = (volatile int64_t*)((char*)origin.data() + alloc_bytes);
        for (auto ptr = (volatile int64_t*)(origin.data()); ptr < end_ptr; ptr++)
        {
            [[maybe_unused]] volatile int64_t val = *ptr;
	    *ptr = 777;
        }
    }

    {
        std::cout << "\nwaiting..." << std::endl;
        std::int32_t n{ 0 };
        std::cin >> n;
    }

    return 0;
}
