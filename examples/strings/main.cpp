/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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
    std::cout << name << "  SEARCH  IN:  " << string << "  SUB_STRING  " << sub_string << std::endl;
    std::cout << "RESULT  POSITION:  " << i << std::endl;
    std::cout << "BENCHMARK  " << name << "  SUB_STRING  SEARCH:  "
              << fun_benchmark.get_last_interval() << "\n" << std::endl;
}
