/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <chrono>
#include <cstring>
#include <iostream>

#include "operations_with_numbers.h"

typedef double data_type;
typedef int natural_data_type;

template<class F, class T>
void test_fun(const std::string& comment, F fun, T n);

template<class F, class T>
void test_fun(const std::string& comment, F fun, T p1, T p2);

template<class F, class T>
void test_largest_common_divisor(const std::string& comment, F fun, T a, T b);

typedef data_type (*prt_fun_1)(data_type);
typedef natural_data_type (*prt_fun_2)(natural_data_type, natural_data_type);
typedef data_type (*prt_fun_3)(data_type, data_type);

void skip_line()
{
    std::cout << std::endl;
}

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    // test fibonacci
    test_fun<prt_fun_1>("RECURSIVE FIBONACCI", fibonacci_recursive, 35);
    test_fun<prt_fun_1>("MATRIX FIBONACCI", fibonacci_matrix, 35);
    
    skip_line();
    
    // test factorial
    test_fun<prt_fun_1>("RECURSIVE FACTORIAL", factorial_recursive, 5);
    test_fun<prt_fun_1>("CYCLIC FACTORIAL", factorial_cyclic, 5);
    test_fun<prt_fun_1>("STIRLING FACTORIAL", factorial_stirling, 5);
    test_fun<prt_fun_1>("SUM DIVIDED BY THE FACTORIAL", sum_divided_by_the_factorial, 5.);
    
    skip_line();
    
    test_fun<prt_fun_2>("POW SIMPLE", pow_simple, 2, 16);
    test_fun<prt_fun_2>("POW ADVANCE", pow_advance, 2, 16);
    test_fun<prt_fun_3>("POW STL", pow, 2, 16);
    
    skip_line();
    
    // test lowest common divisor
    test_largest_common_divisor<prt_fun_2>("SIMPLE", largest_common_divisor, 90, 100);
    test_largest_common_divisor<prt_fun_2>("EUCLID", largest_common_divisor_euclid, 100, 90);
    test_largest_common_divisor<prt_fun_2>("MOD EUCLID", largest_common_divisor_modified_euclid, 90, 100);
    
    return 0;
}

template<class F, class T>
void test_fun(const std::string& comment, F fun, T n)
{
    T current_n = 0;
	std::chrono::high_resolution_clock clock;
	auto start = clock.now();
	{
        current_n = static_cast<T>(fun(n));
    }
	auto end = clock.now();
    std::cout << "BENCHMARK  " << comment << "  n:  " << n <<  "  -  " << current_n
		      << "  TIME:  " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;
}

template<class F, class T>
void test_fun(const std::string& comment, F fun, T p1, T p2)
{
    T current_n = 0;
	std::chrono::high_resolution_clock clock;
	auto start = clock.now();
	{
        current_n = static_cast<T>(fun(p1, p2));
    }
	auto end = clock.now();
    std::cout << "BENCHMARK  " << comment << "  p1:  " << p1 
              << "  and  p2:  " << p2 <<  "  -  " << current_n
			  << "  TIME:  " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;
}

template<class F, class T>
void test_largest_common_divisor(const std::string& comment, F fun, T a, T b)
{
    T current_divisor = fun(a, b);
    std::cout << "Largest common divisor " << comment << "  from:  " 
              << a << "  and  " << b <<  "  -  " << current_divisor << std::endl;    
}
