#include <sched.h>
#include <atomic>
#include <vector>
#include <thread>
#include <sstream>
#include <iostream>

int main()
{
    std::atomic<int> counter(10);
    std::atomic<int> sync_counter(2);

    auto lambda = [&counter, &sync_counter](std::string name, int ost){
        {
            std::stringstream sstream;
            sstream << "[" << name << "] cpu: " << sched_getcpu() << " ost: " << ost << "\n";
            std::cout << sstream.str();
        }

        sync_counter.fetch_sub(1, std::memory_order_release);

        while(sync_counter.load(std::memory_order_acquire) > 0)
        {
            std::this_thread::yield();
        }

        bool done = false;
        while(!done)
        {
            int const current = counter.load(std::memory_order_acquire);
            {
                std::stringstream sstream;
                sstream << "[" << name << "] cpu: " << sched_getcpu() << " val: " << current << "\n";
                std::cout << sstream.str();
            }
            if(current == 0)
            {
                done = true;
            }
            else if(current % 2 == ost)
            {
                counter.fetch_sub(1, std::memory_order_release);
                std::this_thread::yield();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    };

    std::thread t1([&counter, &sync_counter, lambda](){
        lambda("t1", 0);
    });

    std::thread t2([&counter, &sync_counter, lambda](){
        lambda("t2", 1);
    });

    t1.join();
    t2.join();

    return 0;
}
