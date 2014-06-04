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

#include <stdio.h>
#include <memory>
#include <iostream>

class deleter
{
public:
    void operator() (FILE* file) {
        std::cout << "[delete: " << file << "]\n";
        fclose(file);
    }
};

void use_file(const char *name)
{
    std::unique_ptr<FILE, deleter> ptr(fopen(name, "w"));
    throw std::runtime_error("Runtime error");
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    try {
        use_file("/dev/random");
    }
    catch(const std::exception& e)
    {
        std::cerr << "Catch: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Catch all" << std::endl;
    }

    return 0;
}
