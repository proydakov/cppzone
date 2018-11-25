#include <ratio>
#include <atomic>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

#if defined(__linux__) // any linux distribution

#include <sys/prctl.h>

void set_thread_name(std::string const& name)
{
    prctl(PR_SET_NAME, name.c_str(), 0, 0, 0);
}

#else

void set_thread_name(std::string const& name)
{
}

#endif

template<class Q, class T>
int test_main(T controller, int argc, char* argv[],
    std::uint64_t total_events = 64, std::uint64_t num_readers = (std::thread::hardware_concurrency() - 1), std::uint64_t queue_size = 4096)
{
    std::cout << "usage: app <num readers> <events> * 10^6 <queue_size>" << std::endl;

    // TEST details
    std::uint64_t const NUM_READERS = argc > 1 ? std::stoi(argv[1]) : num_readers;
    std::uint64_t const TOTAL_EVENTS = std::uint64_t(argc > 2 ? std::stoi(argv[2]) : total_events) * std::mega::num;
    std::uint64_t const QUEUE_SIZE = argc > 3 ? std::stoul(argv[3]) : queue_size;

    std::cout << "TEST: 1 writer, "
        << std::to_string(NUM_READERS) << " readers, "
        << std::to_string(TOTAL_EVENTS) << " total events, "
        << QUEUE_SIZE << " queue size"
        << std::endl;

    controller.before_test(NUM_READERS, TOTAL_EVENTS);

    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;

    {
        //std::clog.setstate(std::ios_base::failbit);

        auto queue = Q::make(QUEUE_SIZE);
        std::atomic<std::uint64_t> waitinig_readers_counter{ NUM_READERS };

        std::vector<typename Q::reader_ptr> readers;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            readers.push_back(std::move(queue->create_reader()));
        }

        auto const mask = queue->get_alive_mask();
        std::cout << "queue shared_ptr.use_count() [before]: " << queue.use_count() << " (must be " << (1 + NUM_READERS) << ")" << std::endl;;
        std::cout << "alive mask: " << std::bitset<sizeof(mask) * 8>(mask) << " [" << mask << "]" << std::endl;;

        start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            threads.push_back(std::thread([reader = std::move(readers[i]), &waitinig_readers_counter, TOTAL_EVENTS, &controller](){
                auto const rid = reader->get_id();
                set_thread_name("reader_" + std::to_string(rid));

                waitinig_readers_counter--;

                for (std::uint64_t j = 0; j < TOTAL_EVENTS;)
                {
                    auto pair = reader->try_read();
                    if (pair.first)
                    {
                       controller.check_data(rid, pair.second.get_event());
                        j++;
                    }
                }

                controller.reader_done();
            }));
        }

        threads.push_back(std::thread([queue = std::move(queue), &waitinig_readers_counter, TOTAL_EVENTS, &controller]() {
            set_thread_name("writer");

            while (waitinig_readers_counter > 0);

            for (std::uint64_t j = 0; j < TOTAL_EVENTS; j++)
            {
                typename Q::event_type data(controller.create_data(j));
                queue->write(std::move(data));
            }

            controller.writer_done();
        }));

        for (auto& t : threads) t.join();

        std::cout << "queue shared_ptr.use_count() [after]: " << queue.use_count() << " (must be zero)" << std::endl;;
    }

    stop = std::chrono::high_resolution_clock::now();
    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "TIME: " + std::to_string(milliseconds) + " milliseconds\n";
    std::cout << "PERF: " + std::to_string(double(TOTAL_EVENTS) / milliseconds) + " events/millisecond\n";

    controller.after_test();

    return 0;
}
