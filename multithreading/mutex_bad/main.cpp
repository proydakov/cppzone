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

#include <mutex>
#include <thread>
#include <iostream>
#include <functional>

typedef std::recursive_mutex mutex;

void function(mutex& m)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "try lock in thread: " << m.try_lock() << std::endl;
    try {
        m.unlock();
    }
    catch(std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

int main( int argc, char *argv[] )
{
    (void) argc;
    (void) argv;

    mutex m;
    std::thread thread(function, std::ref(m));
    m.lock();
    std::cout << "try lock in main: " << m.try_lock() << std::endl;
    thread.join();
    std::cout << "try lock after thread: " << m.try_lock() << std::endl;

    return 0;
}
