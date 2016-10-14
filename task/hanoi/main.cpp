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
#include <string>
#include <iostream>

typedef int disk;
typedef std::list<disk> tower;

void hanoi(size_t num, tower& start, tower& middle, tower& end);
void show_tower(const tower& peg);
void show_state(const tower& start, const tower& middle, const tower& end);

tower start_peg;
tower middle_peg;
tower end_peg;

size_t counter = 0;

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    size_t num = 5;
    
    for(size_t i = num; i > 0; --i)
        start_peg.push_back(i);
    
    show_state(start_peg, middle_peg, end_peg);
    hanoi(num, start_peg, middle_peg, end_peg);    
    
    std::cout << "total iterations  :  " << counter << std::endl;
    
    return 0;
}

void hanoi(size_t num, tower& start, tower& middle, tower& end)
{
    if(num == 1) {
        end.push_back(start.back());
        start.pop_back();
        show_state(start_peg, middle_peg, end_peg);
        ++counter;
    }
    else {
        hanoi(num - 1, start, end, middle);
        end.push_back(start.back());
        start.pop_back();
        show_state(start_peg, middle_peg, end_peg);
        ++counter;
        hanoi(num - 1, middle, start, end);
        
    }
}

void show_tower(const tower& peg)
{
    tower::const_iterator end_it = peg.end();
    tower::const_iterator it = peg.begin();
    if(it == end_it)
        std::cout << "noop";
    for(; it != end_it; ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

void show_state(const tower& start, const tower& middle, const tower& end)
{
    std::cout << "start  : ";
    show_tower(start);
    std::cout << "middle : ";
    show_tower(middle);
    std::cout << "end    : ";
    show_tower(end);
    size_t max = 4 + start.size() + middle.size() + end.size();
    for(size_t i = 0; i < max; ++i) {
        std::cout << "---";
    }
    std::cout << std::endl;
}
