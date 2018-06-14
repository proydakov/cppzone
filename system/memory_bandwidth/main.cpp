#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <cstdint>
#include <iostream>

std::int64_t constexpr giga = 1024l * 1024l * 1024l;

class experiment
{
public:
    template<class T>
    experiment(std::vector<T> const& data)
        : m_bytes(data.size() * sizeof(T))
        , m_start(std::chrono::high_resolution_clock::now())
    {
    }

    ~experiment()
    {
        auto end = std::chrono::high_resolution_clock::now();

        auto const delta = end - m_start;
        std::cout << "fill size: " << m_bytes * 1.0 / giga << " Gb time: " << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() << " milliseconds" << std::endl;
        std::cout << "memory_bandwidth: " << m_bytes * 1.0 / giga / std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() * 1000 << " GB/s" << std::endl;
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

    std::int64_t const gb = std::stol(argv[1]);
    std::cout << "memory size: " << gb << " Gb" << std::endl;

    using vec_t = std::vector<std::int64_t>;
    vec_t data;

    std::int64_t const alloc_size = gb * giga / sizeof(vec_t::value_type);

    {
        std::cout << "\nallocating..." << std::endl;
        experiment e(data);
        data.resize(alloc_size, 0);
    }

    vec_t copy;

    {
        std::cout << "\ncopying..." << std::endl;
        experiment e(data);
        copy = data;
    }

    std::int64_t summ = 0;

    {
        std::cout << "\naccumulating..." << std::endl;
        experiment e(data);
        summ = std::accumulate(data.begin(), data.end(), std::int64_t(0));
    }

    {
        std::cout << "\nwaiting..." << std::endl;
        std::int32_t n{ 0 };
        std::cin >> n;
    }

    return summ;
}
