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

#include <atomic>
#include <iostream>

#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <boost/date_time.hpp>

struct data
{
    data() :
        counter(0)
    {
    }

    data(const data& obj) {
        std::cout << "copy" << std::endl;
        counter = obj.counter;
    }

    int counter;
};

class worker
{
public:
    worker() :
        m_working(false)
    {
    }
    ~worker()
    {
        stop();
    }

    void start() {
        bool status = m_working.exchange(true);
        if(!status) {
            m_thread.reset(new boost::thread(boost::bind(&worker::thread_fun, this)));
        }
    }

    void stop()
    {
        m_working.store(false);
        if(m_thread.get()) {
            m_thread->join();
        }
    }

    boost::signals2::signal<void (const data&)> signal;

private:
    void thread_fun()
    {
        data d;
        std::cout << "thread started" << std::endl;
        while(m_working.load()) {
            boost::this_thread::sleep(boost::posix_time::seconds(1));
            ++d.counter;
            signal(d);
        }
        std::cout << "thread finished" << std::endl;
    }

private:
    boost::shared_ptr<boost::thread> m_thread;
    std::atomic<bool> m_working;
};

void handler(const data& obj)
{
    std::cout << "handler: " << obj.counter << std::endl;
}

int main( )
{
    worker w;

    w.signal.connect(boost::bind(&handler, _1));
    w.start();
    boost::this_thread::sleep(boost::posix_time::seconds(15));
    return 0;
}
