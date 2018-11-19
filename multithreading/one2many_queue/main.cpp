#include <atomic>
#include <cmath>
#include <vector>
#include <memory>
#include <iso646.h>

#include <ratio>
#include <string>
#include <bitset>
#include <chrono>
#include <thread>
#include <iostream>

enum : std::uint64_t { CPU_CACHE_LINE_SIZE = 64 };
enum : std::uint64_t { MIN_READER_ID = 0 };
enum : std::uint64_t { MAX_READER_ID = 63 };
enum : std::uint64_t { MIN_EVENT_SEQ_NUM = 1 };
enum : std::uint64_t { DUMMY_EVENT_SEQ_NUM = 0 };
enum : std::uint64_t { EMPTY_DATA_MASK = std::uint64_t(0) };
enum : std::uint64_t { CONSTRUCTED_MASK = std::uint64_t(1) << MAX_READER_ID };

// predeclaration

// queue
template<class event_t>
class one2many_queue;

// reader
template<class event_t>
class one2many_reader;

// guard
template<class event_t>
class one2many_guard;

// bucket
template<class event_t>
struct one2many_bucket_t;

// implementation

// guard
template<class event_t>
class one2many_guard
{
public:
    // ctor
    one2many_guard(one2many_bucket_t<event_t>& bucket, std::uint64_t mask) : m_bucket(&bucket), m_mask(mask)
    {
    }

    // move ctor and assign
    one2many_guard(one2many_guard&& data) : m_bucket(data.m_bucket), m_mask(data.m_mask)
    {
        data.m_mask = EMPTY_DATA_MASK;
    }

    one2many_guard& operator=(one2many_guard&& data)
    {
        if (&data != this)
        {
            m_bucket = data.m_bucket;
            m_mask = data.m_mask;
            data.m_mask = EMPTY_DATA_MASK;
        }
        return *this;
    }

    // copy ctor and assign
    one2many_guard(const one2many_guard&) = delete;
    one2many_guard& operator=(const one2many_guard&) = delete;

    // dtor
    ~one2many_guard()
    {
        if (m_mask != EMPTY_DATA_MASK)
        {
            auto const before = m_bucket->m_mask.fetch_and(~m_mask, std::memory_order_release);
            auto const release_etalon(m_mask | CONSTRUCTED_MASK);

            if (before == release_etalon)
            {
                m_bucket->m_event.~event_t();
                m_bucket->m_mask.store(EMPTY_DATA_MASK, std::memory_order_release);
            }
        }
    }

    event_t const& get_event() const
    {
        return m_bucket->m_event;
    }

private:
    one2many_bucket_t<event_t>* m_bucket;
    std::uint64_t m_mask;
};

// reader
template<class event_t>
class one2many_reader
{
public:
    // ctor
    one2many_reader(std::shared_ptr<one2many_queue<event_t>> queue, std::vector<one2many_bucket_t<event_t>>& storage, std::uint64_t read_from, std::uint64_t mask) :
        m_next_read_index(read_from), m_storage(storage), m_mask(mask), m_queue(std::move(queue))
    {
        std::clog << "one2many_reader() id: [" + std::to_string(get_id()) + "]\n";
    }

    // copy ctor and assign
    one2many_reader(const one2many_reader&) = delete;
    one2many_reader& operator=(const one2many_reader&) = delete;

    // move ctor and assign
    one2many_reader(one2many_reader&&) = delete;
    one2many_reader& operator=(one2many_reader&&) = delete;

    // dtor
    ~one2many_reader()
    {
        std::clog << "~one2many_reader() id: [" + std::to_string(get_id()) + "]\n";
    }

    std::pair<bool, one2many_guard<event_t>> try_read()
    {
        bool result = false;
        auto& bucket = m_storage[get_bounded_index(m_next_read_index)];
        if (bucket.m_seqn.load(std::memory_order_acquire) == m_next_read_index)
        {
            m_next_read_index++;
            result = true;
        }
        return std::make_pair(result, one2many_guard<event_t>(bucket, result ? m_mask : EMPTY_DATA_MASK));
    }

    std::size_t get_id() const
    {
        return std::log2(m_mask);
    }

private:
    std::uint64_t get_bounded_index(std::uint64_t seqn) const
    {
        return seqn % m_storage.size();
    }

private:
    std::uint64_t m_next_read_index;
    std::vector<one2many_bucket_t<event_t>>& m_storage;
    std::uint64_t m_mask;
    std::shared_ptr<one2many_queue<event_t>> m_queue;
};

// bucket
template<class event_t>
struct alignas(CPU_CACHE_LINE_SIZE) one2many_bucket_t
{
    one2many_bucket_t() : m_seqn(DUMMY_EVENT_SEQ_NUM), m_mask(EMPTY_DATA_MASK)
    {
    }

    std::atomic<std::uint64_t> m_seqn;
    std::atomic<std::uint64_t> m_mask;
    event_t m_event;
};

static_assert(sizeof(one2many_bucket_t<void*>) == CPU_CACHE_LINE_SIZE, "Performance optimization");

// queue
template<class event_t>
class alignas(CPU_CACHE_LINE_SIZE) one2many_queue : public std::enable_shared_from_this<one2many_queue<event_t>>
{
public:
    ~one2many_queue()
    {
        std::clog << "~one2many_queue()\n";
    }

    one2many_queue(const one2many_queue&) = delete;
    one2many_queue& operator=(const one2many_queue&) = delete;

    one2many_queue(one2many_queue&&) = delete;
    one2many_queue& operator=(one2many_queue&&) = delete;

    // allocate new queue.
    static std::shared_ptr<one2many_queue<event_t>> make(std::size_t n)
    {
        struct make_shared_enabler : public one2many_queue<event_t>
        {
            make_shared_enabler(std::size_t n) : one2many_queue<event_t>(n)
            {
            }
        };

        return std::make_shared<make_shared_enabler>(n);
    }

    // create new reader. Max reader id: MAX_READER_ID
    std::unique_ptr<one2many_reader<event_t>> create_reader()
    {
        /// @todo : think about the ability to dynamically create readers

        auto const next_id = m_shared.m_next_reader_id.fetch_add(1, std::memory_order_seq_cst);;
        if (next_id > MAX_READER_ID)
        {
            throw std::runtime_error("Next reader id overflow: " + std::to_string(next_id) + " > " + std::to_string(MAX_READER_ID));
        }

        // read next_seq_num before updating alive mask
        auto const read_from = m_local.m_next_seq_num;

        std::uint64_t const mask(std::uint64_t(1) << next_id);

        //m_shared.m_alive_mask |= mask;
        //m_shared.m_alive_mask.fetch_or(mask, std::memory_order_seq_cst);
        //m_local.m_alive_mask |= mask;
        m_local.m_alive_mask.fetch_or(mask, std::memory_order_seq_cst);

        std::unique_ptr<one2many_reader<event_t>> reader(new one2many_reader<event_t>(shared_from_this(), m_local.m_storage, read_from, mask));
        return reader;
    }

    bool try_write(event_t&& event)
    {
        auto& bucket = m_local.m_storage[get_bounded_index(m_local.m_next_seq_num)];
        if (bucket.m_mask.load(std::memory_order_acquire) == EMPTY_DATA_MASK)
        {
            //auto const alive_mask = m_shared.m_alive_mask;
            //auto const alive_mask = m_shared.m_alive_mask.load(std::memory_order_acquire);
            //auto const alive_mask = m_local.m_alive_mask;
            auto const alive_mask = m_local.m_alive_mask.load(std::memory_order_acquire);

            bucket.m_event = std::move(event);
            bucket.m_mask.store(alive_mask, std::memory_order_release);
            bucket.m_seqn.store(m_local.m_next_seq_num++, std::memory_order_release);

            return true;
        }
        else
        {
            return false;
        }
    }

    void write(event_t&& obj)
    {
        while (not try_write(std::move(obj)));
    }

    std::uint64_t size() const
    {
        return m_local.m_storage.size();
    }

    std::uint64_t get_alive_mask() const
    {
        //return m_shared.m_alive_mask;
        //return m_shared.m_alive_mask.load(std::memory_order_acquire);
        //return m_local.m_alive_mask;
        return m_local.m_alive_mask.load(std::memory_order_acquire);
    }

private:
    one2many_queue(std::size_t n) : m_local(n)
    {
        std::clog << "one2many_queue() capacity: [" + std::to_string(n) + "]\n";
    }

    std::uint64_t get_bounded_index(std::uint64_t seqn) const
    {
        return seqn % m_local.m_storage.size();
    }

private:
    // object shared between writer and readers threads
    struct shared_t
    {
        shared_t() : m_next_reader_id(MIN_READER_ID)//, m_alive_mask(CONSTRUCTED_MASK)
        {
        }

        std::atomic<std::uint64_t> m_next_reader_id;
        //std::uint64_t m_alive_mask;
        //std::atomic<std::uint64_t> m_alive_mask;
    };

    // object used only in local writer thread
    struct alignas(CPU_CACHE_LINE_SIZE) local_t
    {
        local_t(std::size_t n) : m_next_seq_num(MIN_EVENT_SEQ_NUM), m_storage(n), m_alive_mask(CONSTRUCTED_MASK)
        {
        }

        std::uint64_t m_next_seq_num;
        std::vector<one2many_bucket_t<event_t>> m_storage;
        //std::uint64_t m_alive_mask;
        std::atomic<std::uint64_t> m_alive_mask;
    };

    // enable_shared_from_this data here

    shared_t m_shared;

    // aligned empty space

    local_t  m_local;
};

static_assert(sizeof(one2many_queue<void*>) == CPU_CACHE_LINE_SIZE * 2, "Performance optimization");

// main test code

struct alignas(CPU_CACHE_LINE_SIZE) stat_local_t
{
    std::int64_t counter = 0;
};

struct alignas(CPU_CACHE_LINE_SIZE) stat_global_t
{
    std::atomic<std::int64_t> counter = 0;
};

thread_local stat_local_t g_local_allocated;
thread_local stat_local_t g_local_released;

stat_global_t g_global_allocated;
stat_global_t g_global_released;

struct data_t
{
    // ctor
    data_t() : m_ptr(nullptr)
    {
    }

    data_t(std::uint64_t* ptr) : m_ptr(ptr)
    {
        g_local_allocated.counter++;
    }

    // move ctor and assign
    data_t(data_t&& data) : m_ptr(data.m_ptr)
    {
        m_ptr = nullptr;
    }

    data_t& operator=(data_t&& data)
    {
        if (&data != this)
        {
            release();
            m_ptr = data.m_ptr;
            data.m_ptr = nullptr;
        }
        return *this;
    }

    // copy ctor and assign
    data_t(const data_t&) = delete;
    void operator=(const data_t&) = delete;

    // dtor
    ~data_t()
    {
        release();
    }

private:
    void release()
    {
        if (m_ptr != nullptr)
        {
            delete m_ptr;
            m_ptr = nullptr;
            g_local_released.counter++;
        }
    }

private:
    std::uint64_t* m_ptr;
};

int main(int argc, char* argv[])
{
    std::cout << "usage: app <num readers> <events> * 10^6" << std::endl;

    std::cout << "g_counter before: " << (g_global_allocated.counter - g_global_released.counter) << " (must be zero)" << std::endl;

    std::clog.setstate(std::ios_base::failbit);

    // TEST detailsg_global_allocated
    std::uint64_t const NUM_READERS = argc > 1 ? std::stoi(argv[1]) : (std::thread::hardware_concurrency() - 1);
    std::uint64_t const QUEUE_SIZE = 4096;
    std::uint64_t const TOTAL_EVENTS = std::uint64_t(argc > 2 ? std::stoi(argv[2]) : 256) * std::mega::num;

    std::cout << "TEST: 1 writer, " + std::to_string(NUM_READERS) + " readers, " + std::to_string(TOTAL_EVENTS) + " total events" << std::endl;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
    start = std::chrono::high_resolution_clock::now();

    {
        auto queue = one2many_queue<data_t>::make(QUEUE_SIZE);

        std::atomic<std::uint64_t> counter{ NUM_READERS };

        std::vector<std::thread> threads;
        for (std::size_t i = 0; i < NUM_READERS; i++)
        {
            auto reader = queue->create_reader();

            threads.push_back(std::thread([reader = std::move(reader), &counter, TOTAL_EVENTS](){
                std::clog << "started reader thread: [" + std::to_string(reader->get_id()) + "]\n";
                counter--;

                for (std::uint64_t i = 0; i < TOTAL_EVENTS;)
                {
                    auto pair = reader->try_read();
                    if (pair.first)
                    {
                        i++;
                    }
                }

                g_global_released.counter += g_local_released.counter;
                std::clog << "completed reader thread: [" + std::to_string(reader->get_id()) + "]\n";
            }));
        }

        threads.push_back(std::thread([queue = std::move(queue), &counter, TOTAL_EVENTS]() {
            while (counter > 0);

            auto const count = queue.use_count();
            auto const mask = queue->get_alive_mask();

            std::clog << "started writer thread\n";
            std::clog << "queue shared_ptr.use_count(): " + std::to_string(count) + "\n";
            std::clog << "mask: " + std::bitset<64>(mask).to_string() + " [" + std::to_string(mask) + "]\n";

            for (std::uint64_t i = 0; i < TOTAL_EVENTS; i++)
            {
                data_t data(new std::uint64_t(i));
                queue->write(std::move(data));
            }

            g_global_allocated.counter += g_local_allocated.counter;
            std::clog << "done writer thread\n";
        }));

        for (auto& t : threads) t.join();
    }

    stop = std::chrono::high_resolution_clock::now();
    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "TIME: " + std::to_string(milliseconds) + " milliseconds\n";
    std::cout << "PERF: " + std::to_string( double(TOTAL_EVENTS) / milliseconds ) + " events/millisecond\n";
    std::cout << "g_counter after: " << (g_global_allocated.counter - g_global_released.counter) << " (must be zero)" << std::endl;

    return 0;
}
