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

#include <chrono>
#include <thread>
#include <iostream>

#include <string.h>
#include <unistd.h>

int main()
{
    std::chrono::milliseconds dura( 5000 );
    std::this_thread::sleep_for( dura );

    std::size_t size = sizeof(std::size_t);
    std::size_t elements = (1024 / size) * 1024 * 1024 * 2;

    std::cout << "sizeof(std::size_t): " << size << std::endl;
    std::cout << "elements: " << elements << std::endl;

    std::size_t* ptr = new std::size_t[elements];
    for(size_t i = 0; i < elements; i++) {
        *(ptr + i) = i;
    }

    std::cout << "ptr: " << ptr << std::endl;

    size_t n = 16;
    while(n) {
        std::cout << "n: " << n << std::endl;
        if(fork() == 0) {
            delete [] ptr;
        }
        n--;
        std::chrono::milliseconds dura( 1000 );
        std::this_thread::sleep_for( dura );
    }
    return 0;
}
