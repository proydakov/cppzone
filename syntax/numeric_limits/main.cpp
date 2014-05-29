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

#include <limits>
#include <cstddef>
#include <iostream>

int main()
{
    std::cout << "min short:      " << std::dec << std::numeric_limits<short>::min() << std::endl
              << "min int:        " << std::dec << std::numeric_limits<int>::min() << std::endl
              << "min streamsize: " << std::dec << std::numeric_limits<std::streamsize>::min() << std::endl
              << "min size_t:     " << std::dec << std::numeric_limits<std::size_t>::min() << std::endl
              << "min float:      " << std::numeric_limits<float>::min() << std::endl
              << "min double:     " << std::numeric_limits<double>::min() << std::endl;

    std::cout << std::endl;

    std::cout << "max short:      " << std::dec << std::numeric_limits<short>::max() << std::endl
              << "max int:        " << std::dec << std::numeric_limits<int>::max() << std::endl
              << "max streamsize: " << std::dec << std::numeric_limits<std::streamsize>::max() << std::endl
              << "max size_t:     " << std::dec << std::numeric_limits<std::size_t>::max() << std::endl
              << "max float:      " << std::numeric_limits<float>::max() << std::endl
              << "max double:     " << std::numeric_limits<double>::max() << std::endl;
    return 0;
}
