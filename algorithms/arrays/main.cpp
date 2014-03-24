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
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "operations_with_arrays.h"

typedef int data_type;

typedef void (*prt_operation)(const std::vector<data_type>&, const std::vector<data_type>&, std::vector<data_type>&);
void test_operations(const std::string& name, prt_operation operation, const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res);

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& vector)
{
    long size = vector.size();
    ostr << "vector[" << size << "]  ";
    for(int i = 0; i < size; ++i)
        ostr << vector[i];
    return ostr;    
}

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    // create data
    std::vector<data_type> a;
    for(int i = 0; i < 5; ++i) {
        a.push_back(i);
    }
    std::vector<data_type> b;
    for(int i = 3; i < 8; ++i) {
        b.push_back(i);
    }
    std::vector<data_type> res(std::min(a.size(), b.size()));
    
    test_operations("MERGE", merge, a, b, res);
    test_operations("INTERSECTION", &intersection, a, b, res);
    test_operations("SUBTRACTION", &subtraction, a, b, res);
    test_operations("SUBTRACTION", &subtraction, b, a, res);
    
    data_type value = 4;
    std::cout << "input data: " << a << std::endl << "search element: " << value << " position:  " 
              << binary_search(a, value) << std::endl;
    
    return 0;
}

void test_operations(const std::string& name, prt_operation operation, const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res)
{
    std::cout << "TEST  " << name.c_str() << std::endl;
    operation(a, b, res);
    std::cout << "INPUT DATA:  " << std::endl << a << std::endl << b << std::endl;
    std::cout << "RESULT: " << std::endl << res << std::endl;
    std::cout << std::endl;
}
