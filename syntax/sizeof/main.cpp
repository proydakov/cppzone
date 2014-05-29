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

#include <set>
#include <map>
#include <list>
#include <array>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#ifndef _MSC_VER
class N
{
    int k[0];
};
#endif

class O
{
    
};

class A
{
public :
    virtual ~A();
};


class B
{
public:
    virtual ~B();
};


class C
{
public:
    virtual ~C();
};


class D : public A
{
public:
    virtual ~D();
};

class E : public B, public C
{
public:
    virtual ~E();
};

class F : public A, public B, public C
{
public:
    virtual ~F();
};

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    std::cout << "sizeof class:" << std::endl;
    std::cout << "sizeof O:  " << sizeof(O) << std::endl;
    std::cout << "sizeof A:  " << sizeof(A) << std::endl;
    std::cout << "sizeof B:  " << sizeof(B) << std::endl;
    std::cout << "sizeof C:  " << sizeof(C) << std::endl;
    std::cout << "sizeof D:  " << sizeof(D) << std::endl;
    std::cout << "sizeof E:  " << sizeof(E) << std::endl;
    std::cout << "sizeof F:  " << sizeof(F) << std::endl;
    std::cout << std::endl;

#ifndef _MSC_VER
    std::cout << "sizeof null: "<< sizeof(N) << std::endl;
    N* p = new N[10];
    N* pi = p;
    for(size_t i = 0; i < sizeof(p); ++i, ++pi) {
        std::cout << "adress pi:   " << &pi << std::endl;
    }
    std::cout << std::endl;
    delete [] p;
#endif
    
    std::cout << "sizeof types:" << std::endl;
    std::cout << "sizeof int:      " << sizeof(int) << std::endl;
    std::cout << "sizeof long:     " << sizeof(long) << std::endl;
    std::cout << std::endl;

    std::cout << "sizeof ptr types:" << std::endl;
    std::cout << "sizeof void*:    " << sizeof(void*) << std::endl;
    std::cout << "sizeof int*:     " << sizeof(int*) << std::endl;
    std::cout << "sizeof float*:   " << sizeof(float*) << std::endl;
    std::cout << "sizeof double*:  " << sizeof(double*) << std::endl;
    std::cout << "sizeof N*:       " << sizeof(N*) << std::endl;
    std::cout << "sizeof A*:       " << sizeof(A*) << std::endl;
    std::cout << "sizeof B*:       " << sizeof(B*) << std::endl;
    std::cout << "sizeof C*:       " << sizeof(C*) << std::endl;
    std::cout << "sizeof D*:       " << sizeof(D*) << std::endl;
    std::cout << std::endl;
    
    typedef int type;
    std::list<type> list;
    std::vector<type> vector;
    std::stack<type> stack;
    std::queue<type> queue;
    std::priority_queue<type> priority_queue;
    std::set<type> set;
    std::multiset<type> mset;
    std::map<type, type> map;
    std::multimap<type, type> mmap;
    std::unordered_set<type> uset;
    std::unordered_map<type, type> umap;
    std::unordered_multiset<type> umset;
    std::unordered_multimap<type, type> ummap;
    
    std::cout << "sizeof containers:" << std::endl;
    std::cout << "sizeof array<int, 10>:     " << sizeof(std::array<int, 10>) << std::endl;
    std::cout << "sizeof list:               " << sizeof(list) << std::endl;
    std::cout << "sizeof vector:             " << sizeof(vector) << std::endl;
    std::cout << "sizeof stack:              " << sizeof(stack) << std::endl;
    std::cout << "sizeof queue:              " << sizeof(queue) << std::endl;
    std::cout << "sizeof priority_queue:     " << sizeof(priority_queue) << std::endl;
    std::cout << "sizeof set:                " << sizeof(set) << std::endl;
    std::cout << "sizeof multiset:           " << sizeof(mset) << std::endl;
    std::cout << "sizeof map:                " << sizeof(map) << std::endl;
    std::cout << "sizeof multimap:           " << sizeof(mmap) << std::endl;
    std::cout << "sizeof unordered_set:      " << sizeof(uset) << std::endl;
    std::cout << "sizeof unordered_map:      " << sizeof(umap) << std::endl;
    std::cout << "sizeof unordered_multiset: " << sizeof(umset) << std::endl;
    std::cout << "sizeof unordered_multimap: " << sizeof(ummap) << std::endl;
    std::cout << std::endl;

    int* i_ptr = new int[10];
    int int_array_10[10];
    int int_array_16[16];
    int int_array_64[64];

    std::cout << "sizeof arrays:" << std::endl;
    std::cout << "sizeof new int[10]: " << sizeof(i_ptr) << std::endl;
    std::cout << "sizeof int[10]:     " << sizeof(int_array_10) << std::endl;
    std::cout << "sizeof int[16]:     " << sizeof(int_array_16) << std::endl;
    std::cout << "sizeof int[64]:     " << sizeof(int_array_64) << std::endl;

    return 0;
}
