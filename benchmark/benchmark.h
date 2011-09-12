/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

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
