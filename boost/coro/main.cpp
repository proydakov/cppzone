#include <thread>
#include <vector>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <boost/coroutine/asymmetric_coroutine.hpp>

using coroutine_t = typename boost::coroutines::asymmetric_coroutine<std::size_t>::pull_type;
using yield_t     = typename boost::coroutines::asymmetric_coroutine<std::size_t>::push_type;

std::mutex safe_log_mutex;
std::unordered_map<std::thread::id, size_t> thread_ids;

template<class T>
void thread_safe_log(const T& val)
{
    std::lock_guard<std::mutex> guard(safe_log_mutex);
    const size_t id = thread_ids[std::this_thread::get_id()];
    std::cout << "from: " << std::setw(2) << id << " val: " << val << std::endl;
}

void thread_step(coroutine_t& coroutine)
{
    std::stringstream sstream;
    sstream << "thread step: " << coroutine.get();
    thread_safe_log(sstream.str());
    coroutine();
}

void worker(std::mutex& mutex, coroutine_t& coroutine)
{
    thread_safe_log("thread started");
    while(true) {
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(!coroutine) {
                break;
            }
            thread_step(coroutine);
        }
    }
    thread_safe_log("thread finished");
}

void functor(yield_t& yield)
{
    thread_safe_log("coroutine started");
    for (std::size_t i = 0; i < 10; ++i) {
        std::stringstream sstream;
        sstream << "coroutine generate: " << i;
        thread_safe_log(sstream.str());
        yield(i);
    }
    thread_safe_log("coroutine finished");
}

int main()
{
    std::mutex mutex;
    coroutine_t coroutine(functor);

    std::vector<std::thread> threads;
    for(size_t i = 1; i < 1 + std::thread::hardware_concurrency(); i++) {
        std::thread thread(std::bind(&worker, std::ref(mutex), std::ref(coroutine)));
        thread_ids[thread.get_id()] = i;
        threads.push_back(std::move(thread));
    }

    for(size_t i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    return 0;
};
