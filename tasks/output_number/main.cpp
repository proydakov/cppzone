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
