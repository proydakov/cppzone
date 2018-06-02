#include <sched.h>

#include <cerrno>
#include <cstring>
#include <cstdint>

#include <atomic>
#include <thread>
#include <sstream>
#include <iostream>

void set_name(const char* name)
{
    int ret{0};

    pthread_t this_thread = pthread_self();

    // Attempt to set thread name
    ret = pthread_setname_np(this_thread, name);
    if (ret != 0) {
        std::cout << "Couldn't set thread name. error: " << strerror( errno ) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void set_realtime_priority()
{
     int ret{0};

     // We'll operate on the currently running thread.
     pthread_t this_thread = pthread_self();

     // struct sched_param is used to store the scheduling priority
     struct sched_param params;

     // We'll set the priority to the maximum.
     params.sched_priority = sched_get_priority_min(SCHED_FIFO);

     // Attempt to set thread real-time priority to the SCHED_FIFO policy
     ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
     if (ret != 0) {
         // Print the error
         std::cout << "Unsuccessful in setting thread realtime prio. error: " << strerror( errno ) << std::endl;
         exit(EXIT_FAILURE);
     }

     // Now verify the change in thread priority
     int policy = 0;
     ret = pthread_getschedparam(this_thread, &policy, &params);
     if (ret != 0) {
         std::cout << "Couldn't retrieve real-time scheduling paramers. error: " << strerror( errno ) << std::endl;
         exit(EXIT_FAILURE);
     }

     // Check the correct policy was applied
     if(policy != SCHED_FIFO) {
         std::cout << "Scheduling is NOT SCHED_FIFO!" << std::endl;
     }
     else {
         std::cout << "SCHED_FIFO OK" << std::endl;
     }

     // Print thread scheduling priority
     std::cout << "Thread priority is " << params.sched_priority << std::endl; 
}

void bind_core(std::size_t core_id)
{
    int ret{0};

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    // Attempt to set setaffinity
    ret = sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    if (ret != 0) {
        std::cout << "Couldn't setaffinity. error: " << strerror( errno ) << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <core_id>" << std::endl;
        return EXIT_FAILURE;
    }

    std::size_t const core_id = std::stoul(argv[1]);
    bind_core(core_id);

    auto lambda = [](std::atomic<int>& counter, std::atomic<int>& sync_counter, std::string name, int ost)
    {
        set_name(name.c_str());

        {
            std::stringstream sstream;
            sstream << "[" << name << "] cpu: " << sched_getcpu() << " init. ost: " << ost;
            std::cout << sstream.str() << std::endl;;
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
                sstream << "[" << name << "] cpu: " << sched_getcpu() << " val: " << current;
                std::cout << sstream.str() << std::endl;;
            }

            for(std::int64_t i = 0; i < 4l * std::giga::num; i++);

            if(0 == current)
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

    set_realtime_priority();

    std::atomic<int> counter(10);
    std::atomic<int> sync_counter(2);

    std::thread t2([&counter, &sync_counter, lambda](){
        lambda(counter, sync_counter, "t2", 1);
    });

    std::thread t1([&counter, &sync_counter, lambda](){
        lambda(counter, sync_counter, "t1", 0);
    });

    t1.join();
    t2.join();

    return 0;
}
