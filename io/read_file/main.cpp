/*
 *  Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

void read_file(const std::string& path, std::vector<int>& buffer)
{
    std::ifstream in(path, std::ios::binary);
    if(in.is_open()) {
        size_t i = 0;
        while(!in.eof()) {
            buffer[i] = in.get();
            i++;
        }
    }
    else {
        std::cerr << "Open fire: " << path << " error" << std::endl;
    }
    in.close();
}

template<class binder>
long benchmark(binder bind)
{
    std::chrono::high_resolution_clock clock;
    auto start = clock.now();
    {
        bind();
    }
    auto end = clock.now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return count;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_file>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string path(argv[1]);

    const size_t iters = 10;
    std::vector<long> results(10, 0);

    std::size_t size = sizeof(int);
    std::size_t elements = (1024 / size) * 1024 * 1024 * 2;
    std::vector<int> buffer(elements, 0);
    for(size_t i = 0; i <  elements; i++) {
        buffer[i] = i;
    }

    for(size_t i = 0; i < iters; i++) {
        auto binder = std::bind(read_file, path, std::ref(buffer));
        auto result = benchmark(binder);
        results[i] = result;
    }
    auto min = *(std::min_element(results.begin(), results.end()));
    auto max = *(std::max_element(results.begin(), results.end()));
    auto medium = std::accumulate(results.begin(),results.end(), 0) / results.size();

    std::cout << "benchmark:" << std::endl;
    for(size_t i = 0; i < iters; i++) {
        std::cout << "read file " << i << ": " << results[i] << " microseconds" << std::endl;
    }
    std::cout << "min: " << min << std::endl;
    std::cout << "max: " << max << std::endl;
    std::cout << "med: " << medium << std::endl;

    return EXIT_SUCCESS;
}
