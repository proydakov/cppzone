#include <chrono>
#include <vector>
#include <iostream>
#include <boost/coroutine/asymmetric_coroutine.hpp>

using coroutine_t = typename boost::coroutines::asymmetric_coroutine<std::size_t>::pull_type;
using yield_t     = typename boost::coroutines::asymmetric_coroutine<std::size_t>::push_type;

void functor(yield_t& yield)
{
    while (true) {
        yield(0);
    }
}

int main()
{
    std::chrono::high_resolution_clock clock;
    auto start = clock.now();
    std::vector<coroutine_t> coroutines;
    for(size_t i = 0; i < 1000000; i++) {
        coroutine_t coroutine(functor);
        coroutines.push_back(std::move(coroutine));
    }
    auto end = clock.now();
    std::cout << "process time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;

    int res;
    std::cin >> res;
    return res;
};
