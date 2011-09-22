/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <vector>
#include <iostream>

#include "operations_with_arrays.h"

typedef int data_type;

typedef void (*prt_operation)(const std::vector<data_type>&, const std::vector<data_type>&, std::vector<data_type>&);
void test_operations(const std::string& name, prt_operation operation, const std::vector<data_type> a, const std::vector<data_type> b, std::vector<data_type> res);

template<typename T>
void print_vector(const std::vector<T>& vector)
{
    long size = vector.size();
    std::cout << "vector[" << size << "]  ";
    for(int i = 0; i < size; ++i)
        std::cout << vector[i];
    std::cout << std::endl;
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
    for(int i = 3; i < 6; ++i) {
        b.push_back(i);
    }
    std::vector<data_type> res;
    
    test_operations("MERGE", merge, a, b, res);
    test_operations("INTERSECTION", intersection, a, b, res);
    test_operations("SUBTRACTION", subtraction, a, b, res);
    test_operations("SUBTRACTION", subtraction, b, a, res);
    
    return 1;
}

void test_operations(const std::string& name, prt_operation operation, const std::vector<data_type> a, const std::vector<data_type> b, std::vector<data_type> res)
{
    std::cout << "TEST  " << name << std::endl;
    operation(a, b, res);
    std::cout << "INPUT DATA:  " << std::endl;
    print_vector(a);
    print_vector(b);
    std::cout << "RESULT" << std::endl;
    print_vector(res);
    std::cout << std::endl;
}
