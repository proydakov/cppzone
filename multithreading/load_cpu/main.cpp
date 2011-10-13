/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <boost/thread.hpp>

void load_stream()
{
    while(1);
}

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    // for core-i7
    int thread_col = 8;

    boost::thread my_thread[thread_col];
    for(int i = 0; i < thread_col; ++i) {
        my_thread[i] = boost::thread(&load_stream);
    }

    for(int i = 0; i < thread_col; ++i) {
        my_thread[i].join();
    }

    return 0;
}
