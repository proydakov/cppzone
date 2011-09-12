/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <benchmark.h>
#include <boost/date_time/posix_time/posix_time.hpp>

struct benchmark::data
{
    boost::posix_time::ptime ptime;
    timeinterval m_last_interval;
};

benchmark::benchmark()
{
    m_data = new data;
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
    return m_data->m_last_interval;
}
