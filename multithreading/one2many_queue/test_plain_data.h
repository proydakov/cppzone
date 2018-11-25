#pragma once

// main test code

struct data_t
{
    // ctor
    data_t()
    {
    }

    data_t(std::uint64_t value) : m_value(value)
    {
    }

    // move ctor and assign
    data_t(data_t&& data) : m_value(data.m_value)
    {
        data.m_value = 0;
    }

    data_t& operator=(data_t&& data)
    {
        if (&data != this)
        {
            m_value = data.m_value;
            data.m_value = 0;
        }
        return *this;
    }

    // copy ctor and assign
    data_t(const data_t&) = delete;
    void operator=(const data_t&) = delete;

    // dtor
    ~data_t()
    {
    }

private:
    std::uint64_t m_value;
};

struct perf_plain_test
{
    data_t create_data(std::uint64_t i)
    {
        return data_t(i);
    }

    void check_data(std::uint64_t, data_t const&)
    {
    }

    void before_test(std::uint64_t, std::uint64_t)
    {
    }

    void after_test()
    {
    }

    void reader_done()
    {
    }

    void writer_done()
    {
    }
};
