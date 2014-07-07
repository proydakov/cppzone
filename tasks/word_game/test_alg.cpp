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

#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "config_path.h"
#include "word_game_app.h"

template<class T>
void check_word_pair(const T& first, const T& second)
{
    if (first.size() != second.size()) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " first.size() != second.size()";
        throw std::runtime_error(sstream.str());
    }
    size_t size = first.size();
    int diff = 0;
    for (size_t i = 0; i < size; i++) {
        if (first[i] != second[i]) {
            diff++;
        }
        if (diff > 1) {
            std::stringstream sstream;
            sstream << "Error: " << __FUNCTION__ << " first and second: differ by more than one letter";
            throw std::runtime_error(sstream.str());
        }
    }
}

void test_alg_solution_found(const std::string& task_path, const std::string& dict_path)
{
    WordGameApp app;
    data_t solution;

    auto start = std::chrono::high_resolution_clock::now();
    {
        app.solve(task_path, dict_path, solution);
    }
    auto end = std::chrono::high_resolution_clock::now();

    if (solution.size() < 2) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " solution size < 2";
        throw std::runtime_error(sstream.str());
    }

    data_t task;
    app.get_task(task);

    data_t dict;
    app.get_dict(dict);

    if (task[0] != solution[0]) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " bad solution: first element != task begin";
        throw std::runtime_error(sstream.str());
    }

    auto sln_size = solution.size();
    if (task[1] != solution[sln_size - 1]) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " bad solution: last element != task end";
        throw std::runtime_error(sstream.str());
    }

    for (size_t i = 0; i < sln_size - 1; i++) {
        check_word_pair(solution[i], solution[i + 1]);
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total process time: " << duration << " milliseconds \n" << std::endl;
    app.debug_solve();

    std::cout << "Success: " << __FUNCTION__ << " task path: " << task_path << " dict path: " << dict_path << std::endl;
}

void test_alg_solution_not_found(const std::string& task_path, const std::string& dict_path)
{
    WordGameApp app;
    data_t solution;

    app.solve(task_path, dict_path, solution);

    if (!solution.empty()) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " bad solution: not empty";
        throw std::runtime_error(sstream.str());
    }

    std::cout << "Success: " << __FUNCTION__ << " task path: " << task_path << " dict path: " << dict_path << std::endl;
}

void test_alg_solution_found_impl(const std::string& task_name, const std::string& dict_name)
{
    std::stringstream task;
    task << DATA_DIRECTORY << "/" << task_name;

    std::stringstream dict;
    dict << DATA_DIRECTORY << "/" << dict_name;

    test_alg_solution_found(task.str(), dict.str());
}

void test_alg_solution_not_found_impl(const std::string& task_name, const std::string& dict_name)
{
    std::stringstream task;
    task << DATA_DIRECTORY << "/" << task_name;

    std::stringstream dict;
    dict << DATA_DIRECTORY << "/" << dict_name;

    test_alg_solution_not_found(task.str(), dict.str());
}

int main()
{
    setlocale(LC_ALL, "RUS");

    test_alg_solution_found_impl("task1.txt", "ru_dict_7.txt");
    test_alg_solution_found_impl("task2.txt", "ru_dict_15.txt");
    test_alg_solution_found_impl("task3.txt", "ru_dict_5000.txt");
    test_alg_solution_found_impl("task4.txt", "ru_dict_5000.txt");
    test_alg_solution_found_impl("task5.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task6.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task7.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task8.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task9.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task10.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task11.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task12.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task13.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task14.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task15.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task16.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task17.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task18.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task19.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task20.txt", "ru_dict_32000.txt");
    test_alg_solution_found_impl("task21.txt", "en_dict_3000.txt");

    test_alg_solution_not_found_impl("task_bad1.txt", "ru_dict_32000.txt");
    test_alg_solution_not_found_impl("task_bad2.txt", "ru_dict_32000.txt");

    return EXIT_SUCCESS;
}
