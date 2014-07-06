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

#ifndef I_WORD_GAME_SOLVER
#define I_WORD_GAME_SOLVER

#include <string>
#include <vector>
#include <unordered_set>

class WordGameSolver
{
public:
    static void solve(const std::vector<std::string>& task, const std::vector<std::string>& dict, std::vector<std::string>& solution);

private:
    static std::string ru_alphabet;
    static std::string en_alphabet;

private:
    struct node;

    static void findChilds(node* element, const std::unordered_set<std::string>& hash_dict,
                          const std::string& alphabet, std::unordered_set<std::string>& hash_words);
    static void findTarget(node* element, const std::string& target, std::vector<std::string>& solution);
    static void buildSolution(node* target, std::vector<std::string>& solution);
};

#endif // I_WORD_GAME_SOLVER
