/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_BENCHMARK_H
#define I_BENCHMARK_H

namespace benchmark {

typedef long timeinterval;

class benchmark
{
public:
    enum unit_intervals { seconds, milliseconds, microseconds, nanoseconds };
    
public:
    benchmark(unit_intervals unit = nanoseconds);
    ~benchmark();
    
    void start();
    void stop();
    
    timeinterval get_last_interval();
    unit_intervals get_unit();
    
    void set_unit(unit_intervals unit);
    
private:
    struct data;
    data* m_data;
};

} // benchmark

#endif // I_BENCHMARK_H
