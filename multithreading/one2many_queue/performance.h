#include <ratio>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
#include <iostream>

#include <one2many_queue.h>

template<class E, class T>
int performance_main(T perf, int argc, char* argv[])
{
    std::cout << "usage: app <num readers> <events> * 10^6" << std::endl;

    perf.before_test();

    // TEST details
    std::uint64_t const NUM_READERS = argc > 1 ? std::stoi(argv[1]) : (std::thread::hardware_concurrency() - 1);
    std::uint64_t const QUEUE_SIZE = 4096;
    std::uint64_t const TOTAL_EVENTS = std::uint64_t(argc > 2 ? std::stoi(argv[2]) : 256) * std::mega::num;

    std::cout << "TEST: 1 writer, " + std::to_string(NUM_READERS) + " readers, " + std::to_string(TOTAL_EVENTS) + " total events" << std::endl;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;

    {
        std::clog.setstate(std::ios_base::failbit);

        auto queue = one2many_queue<E>::make(QUEUE_SIZE);
        std::atomic<std::uint64_t> waitinig_readers_counter{ NUM_READERS };

        std::vector<std::unique_ptr<one2many_reader<E>>> readers;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            readers.push_back(std::move(queue->create_reader()));
        }

        auto const count = queue.use_count();
        auto const mask = queue->get_alive_mask();
        std::cout << "queue shared_ptr.use_count(): " + std::to_string(count) << std::endl;;
        std::cout << "alive mask: " + std::bitset<sizeof(mask) * 8>(mask).to_string() + " [" + std::to_string(mask) << std::endl;;

        start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            threads.push_back(std::thread([reader = std::move(readers[i]), &waitinig_readers_counter, TOTAL_EVENTS, &perf](){
                waitinig_readers_counter--;

                for (std::uint64_t i = 0; i < TOTAL_EVENTS;)
                {
                    auto pair = reader->try_read();
                    if (pair.first)
                    {
                        i++;
                    }
                }

                perf.reader_done();
            }));
        }

        threads.push_back(std::thread([queue = std::move(queue), &waitinig_readers_counter, TOTAL_EVENTS, &perf]() {
            while (waitinig_readers_counter > 0);

            for (std::uint64_t i = 0; i < TOTAL_EVENTS; i++)
            {
                E data(perf.create_data(i));
                queue->write(std::move(data));
            }

            perf.writer_done();
        }));

        for (auto& t : threads) t.join();
    }

    stop = std::chrono::high_resolution_clock::now();
    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "TIME: " + std::to_string(milliseconds) + " milliseconds\n";
    std::cout << "PERF: " + std::to_string(double(TOTAL_EVENTS) / milliseconds) + " events/millisecond\n";

    perf.after_test();

    return 0;
}

