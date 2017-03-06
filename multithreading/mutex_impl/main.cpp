/*
 *  Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
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
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <iostream>

#include <unistd.h>
#include <sys/syscall.h>
#include <linux/futex.h>

static int futex(int *uaddr, int futex_op, int val)
{
    return syscall(SYS_futex, uaddr, futex_op, val);
}

class ispinlock
{
public:
    ispinlock() : locked(0)
    {
    }

    ~ispinlock()
    {
    }

    void lock()
    {
        int exp = 0;
        while(!locked.compare_exchange_weak(exp, 1, std::memory_order_acquire)) {
            exp = 0;
        }
    }

    void unlock()
    {
        locked.store(0, std::memory_order_release);
    }

private:
    std::atomic<int> locked;
};

class imutex
{
public:
    imutex() : locked(0), counter(0)
    {
    }

    ~imutex()
    {
    }

    void lock()
    {
        counter.fetch_add(1, std::memory_order_release);
        int exp = 0;
        while(!locked.compare_exchange_weak(exp, 1, std::memory_order_acquire)) {
            exp = 0;
            futex( (int*)(&locked), FUTEX_WAIT, 1 );
        }
    }

    void unlock()
    {
        const int left = counter.fetch_add(-1, std::memory_order_release);
        locked.store(0, std::memory_order_release);
        if(left) {
            futex( (int*)(&locked), FUTEX_WAKE, 1 );
        }
    }

private:
    std::atomic<int> locked;
    std::atomic<int> counter;
};

template<class mutex_t>
void test()
{
    mutex_t mutex;
    std::vector<int> data;

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    std::vector<std::thread> threads;
    for(size_t t = 0; t < std::thread::hardware_concurrency(); t++) {
        std::thread thread([&data, &mutex, t]() {
            for(size_t i = 0; i < 10000; i++) {
                std::lock_guard<mutex_t> guard(mutex);
                data.push_back(i);
            }
        });
        threads.push_back(std::move(thread));
    }

    for(size_t t = 0; t < threads.size(); t++) {
        threads[t].join();
    }

    auto end = clock.now();

    long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "mutex type: " << std::setw(12) << typeid(mutex_t).name() << " | duration: " << std::setw(10) << duration << " microseconds | size: " << data.size() << std::endl;
}

int main()
{
    test<ispinlock>();
    test<imutex>();
    test<std::mutex>();

    return 0;
}
