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

#include <list>
#include <memory>
#include <iostream>
#include <algorithm>

#include <data_reader.h>

#include "word_game_solver.h"

void WordGameSolver::solve(const data_t& task, const data_t& dict, data_t& solution)
{
    data_t data;
    DataReader::readFileByLine("ru_alphabet.txt", data);
    if (data.empty()) {
        throw std::runtime_error("Empty ru file alphabet.");
    }
    std::wstring ru_alphabet = data[0];

    DataReader::readFileByLine("en_alphabet.txt", data);
    if (data.empty()) {
        throw std::runtime_error("Empty en file alphabet.");
    }
    std::wstring en_alphabet = data[0];

    std::wstring start = task[0];
    std::wstring target = task[1];

    const std::wstring* alphabet = nullptr;
    if (std::find(ru_alphabet.begin(), ru_alphabet.end(), start[0]) != ru_alphabet.end()) {
        alphabet = &ru_alphabet;
    }
    else if (std::find(en_alphabet.begin(), en_alphabet.end(),start[0]) != en_alphabet.end()) {
        alphabet = &en_alphabet;
    }
    else {
        throw std::runtime_error("Unsupported alphabet.");
    }

    solution.clear();
    auto head = std::shared_ptr<node<std::wstring>>(new node<std::wstring>(start, nullptr));

    std::list<node<std::wstring>*> search_list;
    std::list<node<std::wstring>*> next_search_list;
    search_list.push_back(head.get());

    std::unordered_set<std::wstring> hash_dict(dict.size());
    std::unordered_set<std::wstring> hash_words(dict.size());
    hash_words.insert(head->value);

    for (auto& el : dict) {
        hash_dict.insert(el);
    }

    const size_t dict_size = dict.size();
    for (size_t i = 0; i < dict_size; i++) {
        auto searchListEndIt = search_list.end();
        for (auto it = search_list.begin(); it != searchListEndIt; ++it) {
            findChilds(*it, hash_dict, (*alphabet), hash_words);
            findTarget(*it, target, solution);
            if (!solution.empty()) {
                break;
            }
            next_search_list.insert(next_search_list.end(), (*it)->childs.begin(), (*it)->childs.end());
        }
        if (!solution.empty()) {
            break;
        }
        if (next_search_list.empty()) {
            break;
        }
        search_list = next_search_list;
        next_search_list.clear();
    }
}

void WordGameSolver::findChilds(node<std::wstring>* element, const std::unordered_set<std::wstring>& hash_dict,
                                const std::wstring& alphabet, std::unordered_set<std::wstring>& hash_words)
{
    const auto value = element->value;
    const size_t word_size = value.size();
    const size_t alphabet_size = alphabet.size();
    for (size_t w = 0; w < word_size; w++) {
        auto test_value = value;
        for (size_t a = 0; a < alphabet_size; a++) {
            test_value[w] = alphabet[a];
            if (hash_dict.count(test_value) > 0 && hash_words.count(test_value) == 0) {
                element->childs.push_back(new node<std::wstring>(test_value, element));
                hash_words.insert(test_value);
            }
        }
    }
}

void WordGameSolver::findTarget(node<std::wstring>* element, const std::wstring& target, std::vector<std::wstring>& solution)
{
    for (auto& el : element->childs) {
        if (el->value == target) {
            buildSolution(el, solution);
            return;
        }
    }
}

void WordGameSolver::buildSolution(node<std::wstring>* target, std::vector<std::wstring>& solution)
{
    solution.clear();
    node<std::wstring>* ptr = target;
    while (ptr != nullptr) {
        solution.push_back(ptr->value);
        ptr = ptr->parent;
    }
    std::reverse(solution.begin(), solution.end());
}
