/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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
        return -1;
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
