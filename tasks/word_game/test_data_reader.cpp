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

#include <data_reader.h>
#include <config_path.h>

void test_read_ok(const std::string& name, const std::string& error)
{
    std::stringstream sstream;
    sstream << DATA_DIRECTORY << "/" << name;

    std::vector<std::string> data;
    try {
        DataReader::readTask(sstream.str(), data);
    }
    catch (...) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << ": found error in good example";
        throw std::runtime_error(sstream.str());
    }
    if (data.empty()) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " " << error;
        throw std::runtime_error(sstream.str());
    }
    std::cout << "Success: test_read_ok name: " << name << std::endl;
}

void test_read_error(const std::string& name, const std::string& error)
{
    std::stringstream sstream;
    sstream << DATA_DIRECTORY << "/" << "task_not_found.txt";

    bool ok = false;
    std::vector<std::string> data;
    try {
        DataReader::readTask(sstream.str(), data);
    }
    catch (...) {
        ok = true;
    }
    if (!ok) {
        std::stringstream sstream;
        sstream << "Error: " << __FUNCTION__ << " " << error;
        throw std::runtime_error(sstream.str());
    }
    std::cout << "Success: test_read_error name: " << name << std::endl;
}

int main()
{
    test_read_ok("task1.txt", "read empty task");
    test_read_error("task_not_found.txt", "task file not exist but read success");

    test_read_ok("dict1.txt", "read empty dict");
    test_read_error("dict_not_found.txt", "dict file not exist but read success");

    return EXIT_SUCCESS;
}
