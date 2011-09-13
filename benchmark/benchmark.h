/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_BENCHMARK_H
#define I_BENCHMARK_H

typedef long timeinterval;

class benchmark
{
public:
    benchmark();
    ~benchmark();

    void start();
    void stop();

    timeinterval get_last_interval();

private:
    struct data;
    data* m_data;
};

#endif // I_BENCHMARK_H
