/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

const int CONST = 1000;

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    usleep(10000);

    int* p = new int[32];
    int* t = new int[32];

    delete [ ] p;
    delete [ ] t;

    return 1;
}
