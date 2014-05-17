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

#include <ctime>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>

#include "iarray.h"

typedef int data_type;

typedef void (*prt_operation)(const std::vector<data_type>&, const std::vector<data_type>&, std::vector<data_type>&);
void test_operation(const std::string& name, prt_operation operation, const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res);

long test_remove_duplicate_impl(std::vector<data_type>& data);

void test_operations();
void test_binary_search();
void test_remove_duplicates();

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& vector)
{
    long size = vector.size();
    ostr << "vector[" << size << "] ";
    for(int i = 0; i < size; ++i)
        ostr << vector[i] << " ";
    return ostr;
}

int main( int argc, char *argv[] )
{
    (void) (argc);
    (void) (argv);

    //test_operations();
    //test_binary_search();
    test_remove_duplicates();

    return 0;
}

void test_operation(const std::string& name, prt_operation operation, const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res)
{
    std::cout << "TEST  " << name.c_str() << std::endl;
    operation(a, b, res);
    std::cout << "INPUT DATA:  " << std::endl << a << std::endl << b << std::endl;
    std::cout << "RESULT: " << std::endl << res << std::endl;
    std::cout << std::endl;
}

long test_remove_duplicate_impl(std::vector<data_type>& data, const std::string& comment)
{
    std::chrono::high_resolution_clock clock;
    auto start = clock.now();
    {
        remove_duplicates(data);
    }
    auto end = clock.now();

    long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    return duration;
}

void test_operations()
{
    std::vector<data_type> a;
    for(int i = 0; i < 5; ++i) {
        a.push_back(i);
    }
    std::vector<data_type> b;
    for(int i = 3; i < 8; ++i) {
        b.push_back(i);
    }
    std::vector<data_type> res(std::min(a.size(), b.size()));

    test_operation("MERGE", merge, a, b, res);
    test_operation("INTERSECTION", &intersection, a, b, res);
    test_operation("SUBTRACTION", &subtraction, a, b, res);
    test_operation("SUBTRACTION", &subtraction, b, a, res);
}

void test_binary_search()
{
    std::vector<data_type> data;
    for(int i = 0; i < 100; ++i) {
        data.push_back(i);
    }

    data_type value = 4;
    std::cout << "input data: " << data << std::endl
              << "search element: " << value << " "
              << "position: " << binary_search(data, value) << std::endl;
}

int irand()
{
   return rand() % 1000;
}

void test_remove_duplicates()
{
    std::vector<std::pair<size_t, long>> results;

    srand((unsigned int) time(NULL));

    size_t size = 25;
    size_t step = size;
    for(size_t i = step; i <= size; i += step) {
        std::vector<data_type> etalon_data(i, 0);
        std::generate(etalon_data.begin(), etalon_data.end(), irand);

        std::cout << "before: " << etalon_data << std::endl;

        std::stringstream sstream;
        sstream << i;
        long duration = test_remove_duplicate_impl(etalon_data, sstream.str());
        results.push_back(std::make_pair(i, duration));

        std::cout << "after: " << etalon_data << std::endl;
    }
    for(size_t i = 0; i < results.size(); i++) {
        if(i > 0) {
            std::cout << ", ";
        }
        std::cout << results[i].first;
    }
    std::cout << std::endl;

    for(size_t i = 0; i < results.size(); i++) {
        if(i > 0) {
            std::cout << ", ";
        }
        std::cout << results[i].second;
    }
    std::cout << std::endl;

}
