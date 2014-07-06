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

#include <iostream>

#include "data_reader.h"
#include "data_validator.h"
#include "word_game_solver.h"

#include "word_game_app.h"

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& vector)
{
    for (auto &el : vector) {
        ostr << el << "\n";
    }
    return ostr;
}

void WordGameApp::solve(const std::string& task_path, const std::string& dict_path, std::vector<std::string>& solution)
{
    m_task.clear();
    m_dict.clear();

    DataReader::readTask(task_path, m_task);
    DataValidator::validateTask(m_task);

    DataReader::readTask(dict_path, m_dict);
    DataValidator::validateDict(m_dict);

    WordGameSolver::solve(m_task, m_dict, m_solution);
    solution = m_solution;
}

void WordGameApp::get_task(std::vector<std::string>& task)
{
    task = m_task;
}

void WordGameApp::get_dict(std::vector<std::string>& dict)
{
    dict = m_dict;
}

void WordGameApp::get_solution(std::vector<std::string>& solution)
{
    solution = m_solution;
}

void WordGameApp::debug_solve()
{
    std::cout << "Task: \n";
    std::cout << m_task << "\n";

    //std::cout << "Dict: \n";
    //std::cout << m_dict << "\n";

    std::cout << "Solution: \n";
    std::cout << m_solution << "\n";
}
