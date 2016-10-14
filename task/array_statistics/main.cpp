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

#include <ctime>
#include <vector>
#include <chrono>
#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>

int irandom()
{
    const int max = 1000000;
    return (std::rand() % max) - max / 2;
}

template<class T>
bool is_positive(T t)
{
    return t > 0;
}

template<class T>
bool is_negative(T t)
{
    return t < 0;
}

template<class T>
bool is_zero(T t)
{
    return t == 0;
}

template<class T>
struct statistics_calculator
{
    statistics_calculator() :
        positive(0),
        negative(0),
        zero(0)
    {
    }

    void operator() (T t)
    {
        if(is_positive(t)) {
            ++positive;
        }
        else if(is_negative(t)) {
            ++negative;
        }
        else {
            ++zero;
        }
    }

    size_t positive;
    size_t negative;
    size_t zero;
};

void print_statistics(const std::string& comment, long duration, size_t positive, size_t negative, size_t zero)
{
    std::cout << "statistics " << comment << std::endl;
    std::cout << "benchmark: " << duration << " us" << std::endl;
    std::cout << "positive: " << positive << std::endl;
    std::cout << "negative: " << negative << std::endl;
    std::cout << "zero: " << zero << std::endl;
    std::cout << "sum: " << positive + negative + zero << std::endl;
}

int main( int argc, char *argv[] )
{
    (void) (argc);
    (void) (argv);

    std::srand ( unsigned ( std::time(0) ) );

    typedef int element;
    size_t size = 16 * 1024 * 1024;
    std::vector<element> vector(size, 0);

    std::generate(vector.begin(), vector.end(), irandom);

    {
        size_t positive = 0;
        size_t negative = 0;
        size_t zero = 0;

        std::chrono::high_resolution_clock clock;
        auto start = clock.now();
        {
            positive = std::count_if(vector.begin(), vector.end(), is_positive<element>);
            negative = std::count_if(vector.begin(), vector.end(), is_negative<element>);
            zero = std::count_if(vector.begin(), vector.end(), is_zero<element>);
        }

        auto end = clock.now();
        print_statistics("std::count_if", static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()), positive, negative, zero);
    }
    std::cout << std::endl;
    {
        statistics_calculator<element> calculator;
        std::chrono::high_resolution_clock clock;
        auto start = clock.now();
        {
            calculator = std::for_each(vector.begin(), vector.end(), calculator);
        }

        auto end = clock.now();
        print_statistics("std::for_each", static_cast<long>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()), calculator.positive, calculator.negative, calculator.zero);

    }

    return 0;
}
