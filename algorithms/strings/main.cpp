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

#include <iostream>

#include <benchmark.h>

#include "operations_with_strings.h"

typedef index_type (*prt_fun)(const std::string&, const std::string&);
void test_sub_string(const std::string& name, prt_fun fun, const std::string& string, const std::string& sub_string);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    std::string string("asdfffe45435353eewgw3543534443543ekkegweg543543wefwecweg5443f34r");
    std::string sub_string("kke");
    
    test_sub_string("BF", substring_search_bf, string, sub_string);
    
    return 0;
}

void test_sub_string(const std::string& name, prt_fun fun, const std::string& string, const std::string& sub_string)
{
    index_type i = 0;
    benchmark::benchmark fun_benchmark;
    fun_benchmark.start(); {
        i = fun(string, sub_string);
    }
    fun_benchmark.stop();
    std::cout << name.c_str() << "  SEARCH  IN:  " << string.c_str() 
		      << "  SUB_STRING  " << sub_string.c_str() << std::endl;
    std::cout << "RESULT  POSITION:  " << i << std::endl;
    std::cout << "BENCHMARK  " << name.c_str() << "  SUB_STRING  SEARCH:  "
              << fun_benchmark.get_last_interval() << "\n" << std::endl;
}
