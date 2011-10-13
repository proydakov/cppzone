/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>

#include <benchmark.h>

#include <my_sort.h>

typedef double data_type;

typedef void (*prt_sort)(std::vector<data_type>&);
void test_sort(const std::string& name, prt_sort sort, std::vector<data_type> data, const std::vector<data_type> sorted_data);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    index_type array_size = 3000;
    srand(time(NULL));
    std::vector<data_type> input_data;
    for(int i = 0; i < array_size; ++i) {
        input_data.push_back(rand() % 1000);
    }

    std::cout << "Sort array size:  " << array_size << "  elements\n" << std::endl;

    // ETALON  STD  SORT
    std::vector<data_type> input_standart_sort_data(input_data);
    std::sort(input_standart_sort_data.begin(), input_standart_sort_data.end());

    std::vector<data_type> input_bubble_sort_data(input_data);
    test_sort("BUBBLE", bubble_sort, input_bubble_sort_data, input_standart_sort_data);

    std::vector<data_type> input_shaker_sort_data(input_data);
    test_sort("SHAKER", shaker_sort, input_shaker_sort_data, input_standart_sort_data);

    std::vector<data_type> input_selection_sort_data(input_data);
    test_sort("SELECTION", selection_sort, input_selection_sort_data, input_standart_sort_data);

    std::vector<data_type> input_insertion_sort_data(input_data);
    test_sort("INSERTION", insertion_sort, input_insertion_sort_data, input_standart_sort_data);

    std::vector<data_type> input_shell_sort_data(input_data);
    test_sort("SHELL", shell_sort, input_shell_sort_data, input_standart_sort_data);

    std::vector<data_type> input_merge_sort_data(input_data);
    test_sort("MERGE", merge_sort, input_merge_sort_data, input_standart_sort_data);

    std::vector<data_type> input_quick_sort_data(input_data);
    test_sort("QUICK", quick_sort, input_quick_sort_data, input_standart_sort_data);

    return 0;
}

void test_sort(const std::string& name, prt_sort sort, std::vector<data_type> data, const std::vector<data_type> sorted_data)
{
    benchmark::benchmark sort_benchmark;
    sort_benchmark.start(); {
        sort(data);
    }
    sort_benchmark.stop();
    std::cout << "BENCHMARK  " << name << "  SORT:  " << sort_benchmark.get_last_interval() << std::endl;
    if(data == sorted_data) {
        std::cout << "SORT  " << name << "  WORKING  GOOD\n" << std::endl;
    }
    else {
        std::cout << "SORT  " << name << "  ERROR\n" << std::endl;
    }
}
