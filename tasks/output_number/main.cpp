/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

void recursive_output_number(int num);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    recursive_output_number(10);
    std::cout << std::endl;

    return 1;
}

void recursive_output_number(int num)
{
    if(num < 1)
        return;
    recursive_output_number(num / 2);
    std::cout << num % 2;
}

