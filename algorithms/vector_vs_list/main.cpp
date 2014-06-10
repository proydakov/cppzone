/*
 *  Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <list>
#include <ctime>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    (void) (argc);
    (void) (argv);

    srand((unsigned int)time(NULL));

    std::vector<double> vector_iter_data;
    std::vector<double> vector_index_data;
    std::list<double> list_data;

    size_t data_size = 100000000;
    for(size_t i = 0; i < data_size; i++) {
        double value = static_cast<double>(rand() % 1000) / 1000;
        vector_iter_data.push_back(value);
        vector_index_data.push_back(value);
        list_data.push_back(value);
    }

	std::chrono::high_resolution_clock clock;

    ///////////////////////////////////////////////////////////////////////////

	auto viterclock_start = clock.now();
    double vector_iter_sum = 0;
    auto endVector = vector_iter_data.end();
    for(auto it = vector_iter_data.begin(); it != endVector; ++it) {
        vector_iter_sum += *it;
    }
	auto viterclock_end = clock.now();

    ///////////////////////////////////////////////////////////////////////////

	auto vindexclock_start = clock.now();
    double vector_index_sum = 0;
    for(size_t i = 0; i < data_size; i++) {
        vector_index_sum += vector_index_data[i];
    }
	auto vindexclock_end = clock.now();

    ///////////////////////////////////////////////////////////////////////////

	auto lclock_start = clock.now();
    double list_sum = 0;
    auto endList = list_data.end();
    for(auto it = list_data.begin(); it != endList; ++it) {
        list_sum += *it;
    }
	auto lclock_end = clock.now();

    ///////////////////////////////////////////////////////////////////////////

    std::cout << "sum elements num: " << data_size << std::endl;
	std::cout << "vector iter  process time: " << std::chrono::duration_cast<std::chrono::microseconds>(viterclock_end - viterclock_start).count() << " us" << " result: " << vector_iter_sum << std::endl;
	std::cout << "vector index process time: " << std::chrono::duration_cast<std::chrono::microseconds>(vindexclock_end - vindexclock_start).count() << " us" << " result: " << vector_index_sum << std::endl;
	std::cout << "list process time:         " << std::chrono::duration_cast<std::chrono::microseconds>(lclock_end - lclock_start).count() << " us" << " result: " << list_sum << std::endl;

    return 0;
}
