#include <cmath>
#include <chrono>
#include <vector>
#include <iomanip>
#include <iostream>

[[clang::optnone]]
[[gnu::optimize("-O0")]]
int test(size_t iters, int step)
{
    int val = 0;
    for(size_t i = 0; i < iters; i++) {
        val += step;
    }
    return val;
}

int main(int argc, char* argv[])
{
    const std::vector<size_t> powers{
        6, 7, 8, 9, 10
    };

    int val = 0;
    for(const auto power : powers) {
        size_t item = std::pow(10, power);

        std::chrono::high_resolution_clock clock;
        auto start = clock.now();
        {
            val += test(item, 7);
        }
        auto end = clock.now();
        const size_t time = static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

        std::cout << "iters: " << std::setw(5) << (std::string("10^") + std::to_string(power))
                  << " time: " << std::setw(7) << time << " milliseconds"
                  << std::endl;
    }

    return val;
}
