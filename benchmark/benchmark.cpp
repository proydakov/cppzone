/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <benchmark.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace benchmark {

struct benchmark::data
{
    boost::posix_time::ptime ptime;
    timeinterval m_last_interval;
    benchmark::unit_intervals m_unit;
};

benchmark::benchmark(unit_intervals unit)
{
    m_data = new data;
    m_data->m_unit = unit;
}

benchmark::~benchmark()
{
    delete m_data;
}

void benchmark::start()
{
    m_data->ptime = boost::posix_time::microsec_clock::universal_time();
}

void benchmark::stop()
{
    m_data->m_last_interval = (boost::posix_time::microsec_clock::universal_time() - m_data->ptime).total_nanoseconds();
}

timeinterval benchmark::get_last_interval()
{
    switch(m_data->m_unit) {
    case seconds:
        return m_data->m_last_interval / 1000000000;
    case milliseconds:
        return m_data->m_last_interval / 1000000;
    case microseconds:
        return m_data->m_last_interval / 1000;
    case nanoseconds:
        return m_data->m_last_interval;
    default:
        return 0;
    }
}

benchmark::unit_intervals benchmark::get_unit()
{
    return m_data->m_unit;
}

void benchmark::set_unit(unit_intervals unit)
{
    m_data->m_unit = unit;
}

} // benchmark
