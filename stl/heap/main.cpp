/*
 *  Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <vector>
#include <iostream>
#include <algorithm>

template<typename T>
std::ostream& operator<<(std::ostream& ostr, const std::vector<T>& vector)
{
    const size_t size = vector.size();
    ostr << "vector[" << size << "] ";
    for(size_t i = 0; i < size; ++i)
        ostr << vector[i] << " ";
    return ostr;
}

void heap_1()
{
    std::cout << "heap_1" << std::endl;

    int myints[] = {10, 20, 30, 5, 15, 50, 35, 25};
    std::vector<int> v(myints,myints + 7);
    std::cout << "init" << std::endl;
    std::cout << v << std::endl;
    std::cout << std::endl;

    std::make_heap(v.begin(),v.end());
    std::cout << "make" << std::endl;
    std::cout << v << std::endl;
    std::cout << std::endl;

    std::sort_heap(v.begin(),v.end());
    std::cout << "sort" << std::endl;
    std::cout << v << std::endl;
}

void heap_2()
{
    std::cout << "heap_2" << std::endl;

    std::vector<int> v { 3, 1, 4, 1, 5, 9 };
    std::cout << v << std::endl;

    std::cout << "make" << std::endl;
    std::make_heap(v.begin(), v.end());
    std::cout << v << std::endl;

    int i = 0;
    for(auto it = v.end(), bit = v.begin(); it != bit; --it, ++i) {
        std::cout << "step " << i << std::endl;
        std::pop_heap(v.begin(), it);
        std::cout << v << std::endl;
    }

    std::cout << "result" << std::endl;
    std::cout << v << std::endl;
}

void heap_3()
{
    std::vector<int> v { 3, 1, 4, 1, 5, 9 };
 
    std::make_heap(v.begin(), v.end());
 
    std::cout << "v: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    v.push_back(6);
 
    std::cout << "before push_heap: ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
 
    std::push_heap(v.begin(), v.end());
 
    std::cout << "after push_heap:  ";
    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';
}

int main()
{
    heap_1();

    std::cout << "\n";

    heap_2();

    std::cout << "\n";

    heap_3();

    return 0;
}
