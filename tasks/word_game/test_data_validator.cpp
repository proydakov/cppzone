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
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <data_validator.h>

void test_validate_dict_ok()
{
    data_t dict;
    dict.push_back(istring("robot"));
    dict.push_back(istring("xobot"));
    try {
        DataValidator::validateDict(dict);
    }
    catch (...) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << ": found error in good dict";
        throw std::runtime_error(sstream.str());
    }
    std::cout << "Success: " << __FUNCTION__ << std::endl;
}

void test_validate_dict_error()
{
    data_t dict;

    bool ok = false;
    try {
        DataValidator::validateDict(dict);
    }
    catch (...) {
        ok = true;
    }
    if (!ok) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << ": not found error in bad dict";
        throw std::runtime_error(sstream.str());
    }
    std::cout << "Success: " << __FUNCTION__ << std::endl;
}

int main()
{
    test_validate_dict_ok();
    test_validate_dict_error();

    return EXIT_SUCCESS;
}
