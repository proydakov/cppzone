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

#include <ctime>
#include <vector>
#include <cstdlib>
#include <algorithm>

int irandom()
{
    return (std::rand() % 1000000);
}

int main( int argc, char *argv[] )
{
    (void) (argc);
    (void) (argv);

    std::srand ( unsigned ( std::time(0) ) );

    typedef int element;
    size_t size_element = sizeof(element);
    size_t size = 1024 * 1024 * 1024 / size_element;
    std::vector<element> vector(size, 0);

    std::generate(vector.begin(), vector.end(), irandom);
    std::stable_sort(vector.begin(), vector.end());

    return 0;
}
