#include "test_main.h"

// main test code

struct alignas(CPU_CACHE_LINE_SIZE)stat_local_t
{
    std::int64_t counter = 0;
};

struct alignas(CPU_CACHE_LINE_SIZE)stat_global_t
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
        data.m_ptr = nullptr;
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

struct perf_allocated_test
{
    data_t create_data(std::uint64_t i)
    {
        return data_t(new std::uint64_t(i));
    }

    void before_test()
    {
        std::cout << "g_counter before: " << (g_global_allocated.counter - g_global_released.counter) << " (must be zero)" << std::endl;
    }

    void reader_done()
    {
        g_global_released.counter += g_local_released.counter;
    }

    void writer_done()
    {
        g_global_allocated.counter += g_local_allocated.counter;
    }

    void after_test()
    {
        std::cout << "g_counter after: " << (g_global_allocated.counter - g_global_released.counter) << " (must be zero)" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    perf_allocated_test test;
    return test_main<data_t, perf_allocated_test>(test, argc, argv);
}
