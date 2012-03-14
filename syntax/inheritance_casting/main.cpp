/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <iostream>

struct A
{
    virtual void fun() {
        std::cout << "A::fun" << std::endl;
    }
};

struct B : public A
{
    virtual void fun() {
        std::cout << "B::fun" << std::endl;
    }
};

struct C : public B
{
    virtual void fun() {
        std::cout << "C::fun" << std::endl;
    }
};

struct D : public C
{
    virtual void fun() {
        std::cout << "D::fun" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    A* ab = new B;
    ab->fun();
    static_cast<A*>(ab)->fun();
    dynamic_cast<A*>(ab)->fun();
    reinterpret_cast<A*>(ab)->fun();
    delete ab;
    
    A* ad = new D;
    ad->fun();
    delete ad;
    
    return 0;
}
