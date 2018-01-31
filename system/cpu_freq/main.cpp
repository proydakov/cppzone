#include <ratio>
#include <thread>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include <sched.h>

uint64_t rdtsc()
{
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void bind_core(size_t core_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
}


#pragma GCC push_options
#pragma GCC optimize ("O0")

void cycle(size_t num)
{
    size_t counter1 = 0;
    size_t counter2 = 0;
    for(size_t i = 0; i < num * std::giga::num; i++)
    {
        counter1 += 1;
        counter2 *= 1;
    }
}

#pragma GCC pop_options

uint64_t milliseconds()
{
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return count;
}

class freq_t
{
public:
    freq_t(uint64_t freq) : val_(freq) {}

    uint64_t const val_;
};

std::ostream& operator<<(std::ostream& os, freq_t freq)
{
    os << std::fixed;
    os.precision(6);

    double d = freq.val_;
    if(d > std::giga::num)
    {
        os << d / std::giga::num << " GHz";
    }
    else if(d > std::mega::num)
    {
        os << d / std::mega::num << " MHz";
    }
    else if(d > std::kilo::num)
    {
        os << d / std::kilo::num << " KHz";
    }
    else
    {
        os << d << " Hz";
    }

    return os;
}

int main()
{
    size_t const cores = std::thread::hardware_concurrency();
    for(size_t core = 1; core <= cores; core++)
    {
	bind_core(core - 1);

        cycle(1);

        uint64_t const t1 = milliseconds();
	uint64_t const r1 = rdtsc();

        cycle(2);

        uint64_t const t2 = milliseconds();
        uint64_t const r2 = rdtsc();

        uint64_t const dt = t2 - t1;
        uint64_t const dr = r2 - r1;

        if(0 == dt)
        {
            std::cout << "cpu: " << std::setw(3) << core << " delta time is zero." << std::endl;
            continue;
        }

        uint64_t const freq = dr * std::mega::num / dt;
        std::cout << "cpu: " << std::setw(3) << core << " freq: " << freq_t(freq) << std::endl;
    }

    return 0;
}
