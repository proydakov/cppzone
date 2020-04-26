#include <ratio>
#include <atomic>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

#include "queue_common.h"

#if defined(__linux__) // any linux distribution

#include <x86intrin.h>

#include <sys/prctl.h>

void set_thread_name(std::string const& name)
{
    prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
}

#else

#include <intrin.h>

void set_thread_name(std::string const&)
{
}

#endif

struct alignas(QUEUE_CPU_CACHE_LINE_SIZE) wait_t
{
    long waitCounter = 0;
};

template<class Q, class T>
int test_main(int argc, char* argv[],
    std::uint64_t total_events = 64, std::uint64_t num_readers = (std::thread::hardware_concurrency() - 1), std::uint64_t queue_size = 4096)
{
    std::cout << "usage: app <num readers> <events> * 10^6 <queue_size>" << std::endl;

    // TEST details
    std::uint64_t const NUM_READERS = static_cast<std::uint64_t>(argc > 1 ? std::stoul(argv[1]) : num_readers);
    std::uint64_t const TOTAL_EVENTS = static_cast<std::uint64_t>(argc > 2 ? std::stoul(argv[2]) : total_events) * std::mega::num;
    std::uint64_t const QUEUE_SIZE = argc > 3 ? std::stoul(argv[3]) : queue_size;

    std::cout << "TEST: 1 writer, "
        << std::to_string(NUM_READERS) << " readers, "
        << std::to_string(TOTAL_EVENTS) << " total events, "
        << QUEUE_SIZE << " queue size"
        << std::endl;

    T controller(NUM_READERS, TOTAL_EVENTS);

    std::uint64_t rdtsc_start, rdtsc_end;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;

    long writerWait = 0;
    std::vector<wait_t> readersWait{ NUM_READERS };

    {
        //std::clog.setstate(std::ios_base::failbit);

        Q queue(QUEUE_SIZE);
        std::atomic<std::uint64_t> waitinig_readers_counter{ NUM_READERS };

        std::vector<typename Q::reader_type> readers;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            readers.push_back(std::move(queue.create_reader()));
        }

        auto const mask = queue.get_alive_mask();
        std::cout << "alive mask: " << std::bitset<sizeof(mask) * 8>(mask) << " [" << mask << "]" << std::endl;;

        rdtsc_start = __rdtsc();
        start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            threads.emplace_back([reader = std::move(readers[i]), &waitinig_readers_counter, stat = &readersWait[i], TOTAL_EVENTS, &controller]() mutable {
                auto const rid = reader.get_id();
                set_thread_name("reader_" + std::to_string(rid));

                waitinig_readers_counter--;

                for (std::uint64_t j = 0; j < TOTAL_EVENTS;)
                {
                    auto opt = reader.try_read();
                    if (opt)
                    {
                        j++;
                    }
                    else
                    {
                        stat->waitCounter++;
                        _mm_pause();
                    }
                }

                controller.reader_done();
            });
        }

        threads.emplace_back([&queue, &waitinig_readers_counter, &writerWait, TOTAL_EVENTS, &controller]() mutable {
            set_thread_name("writer");

            while (waitinig_readers_counter > 0);

            for (std::uint64_t j = 0; j < TOTAL_EVENTS; j++)
            {
                typename Q::event_type data(controller.create_data(j));
                while(!queue.try_write(std::move(data), std::memory_order_seq_cst))
                {
                    writerWait++;
                    _mm_pause();
                }
            }

            controller.writer_done();
        });

        for (auto& t : threads) t.join();

        rdtsc_end = __rdtsc();
        stop = std::chrono::high_resolution_clock::now();
    }

    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    auto const rdtsc_delta = rdtsc_end - rdtsc_start;

    std::cout << "W WAIT: " << writerWait << "\n";
    for (auto const& stat : readersWait)
    {
        std::cout << "R WAIT: " << stat.waitCounter << "\n";
    }
    std::cout << "\n";
    std::cout << "TIME: " << milliseconds << " milliseconds\n";
    std::cout << "TIME: " << rdtsc_delta << " cycles\n";
    std::cout << "PERF: " << double(double(TOTAL_EVENTS) / double(milliseconds)) << " events/millisecond\n";
    std::cout << "PERF: " << double(double(rdtsc_delta) / double(TOTAL_EVENTS)) << " cycle/event\n";

    return 0;
}
