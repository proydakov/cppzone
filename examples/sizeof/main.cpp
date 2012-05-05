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

#include <map>
#include <set>
#include <list>
#include <vector>
#include <iostream>

class N
{
#ifndef _MSC_VER
    int k[0];
#endif
};

class O
{
    
};


class A
{
public :
    virtual ~A();
};


class B : public A
{
public:
    virtual ~B();
};


class C : public B
{
public:
    virtual ~C();
};


class D : public B, public C
{
public:
    virtual ~D();
};


class E : public A, public B, public C
{
public:
    virtual ~E();
};


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;    

    std::cout << "sizeof N:  " << sizeof(N) << std::endl;
    std::cout << "sizeof O:  " << sizeof(O) << std::endl;
    std::cout << "sizeof A:  " << sizeof(A) << std::endl;
    std::cout << "sizeof B:  " << sizeof(B) << std::endl;
    std::cout << "sizeof C:  " << sizeof(C) << std::endl;
    std::cout << "sizeof D:  " << sizeof(D) << std::endl;
    std::cout << "sizeof E:  " << sizeof(E) << std::endl;
    
    std::cout << std::endl;
   
    N null;
    
    std::cout << "sizeof null: "<< sizeof(null) << std::endl;
    (void)null;

    int size = 10;
    N* p = new N[size];

    N* pi = p;
    for(int i = 0; i < size; ++i, ++pi)
        std::cout << "adress pi:   " << &pi << std::endl;

    delete [] p;
    
    std::cout << std::endl;
    
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
    std::list<type> l;
    std::vector<type> v;
    std::set<type> s;
    std::map<type, type> m;
    
    std::cout << "sizeof list:    " << sizeof(l) << std::endl;
    std::cout << "sizeof vector:  " << sizeof(v) << std::endl;
    std::cout << "sizeof set:     " << sizeof(s) << std::endl;
    std::cout << "sizeof map:     " << sizeof(m) << std::endl;
    
    return 0;
}
