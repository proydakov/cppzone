#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <fstream>

enum : std::uint64_t { CPU_CACHE_LINE = 64 };

// main test code

struct data_t
{
    // ctor
    data_t(std::chrono::time_point<std::chrono::high_resolution_clock> start) : m_start(start)
    {
    }

    // move ctor and assign
    data_t(data_t&& data) : m_start(data.m_start)
    {
    }

    data_t& operator=(data_t&& data)
    {
        m_start = data.m_start;
        return *this;
    }

    // copy ctor and assign
    data_t(const data_t&) = delete;
    void operator=(const data_t&) = delete;

    // dtor
    ~data_t()
    {
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

struct latency_test
{
    data_t create_data(std::uint64_t i)
    {
        return data_t(std::chrono::high_resolution_clock::now());
    }

    void check_data(std::uint64_t i, data_t const& data)
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        auto const microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - data.m_start).count();
        m_lines[i].m_delta.push_back(microseconds);
    }

    void before_test(std::uint64_t NUM_READERS, std::uint64_t TOTAL_EVENTS)
    {
        m_lines.resize(NUM_READERS);
        for(auto & line : m_lines)
        {
            line.m_delta.reserve(TOTAL_EVENTS);
        }
    }

    void after_test()
    {
        for(std::size_t i = 0; i < m_lines.size(); i++)
        {
            auto const& line = m_lines[i];
            std::ofstream output("reader_" + std::to_string(i));
            for(auto delta : line.m_delta)
            {
                output << delta << "\n";
            }
        }
    }

    void reader_done()
    {
    }

    void writer_done()
    {
    }

private:
    struct alignas(CPU_CACHE_LINE) line_t
    {
        std::vector<std::uint64_t> m_delta;
    };

    std::vector<line_t> m_lines;
};
