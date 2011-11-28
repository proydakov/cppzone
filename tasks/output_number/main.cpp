/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <list>
#include <iostream>

typedef int type;

void recursive_output_number(type num, type base);
void output_number(type num, type base);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    type num = 100;
    type base = 7;
    
    recursive_output_number(num, base);
    std::cout << std::endl;
    output_number(num, base);
    std::cout << std::endl;
    
    return 0;
}

void recursive_output_number(type num, type base)
{
    if(num < 1)
        return;
    recursive_output_number(num / base, base);
    std::cout << num % base;
}

void output_number(type num, type base)
{
    typedef std::list<type> container;
    container list;
    
    while(num != 0) {
        list.push_front(num % base);
        num /= base;
    }
    
    container::const_iterator endIt = list.end();
    for(container::const_iterator it = list.begin(); it != endIt; ++it) {
        std::cout << *it;   
    }
}
