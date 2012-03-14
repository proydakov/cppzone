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
    A() { std::cout << "A"; }
    virtual ~A() { std::cout << "~A"; }
    virtual void fun() {
        std::cout << "A::fun" << std::endl;
    }
};

struct B : virtual public A
{
    B() { std::cout << "B"; }
    virtual ~B() { std::cout << "~B"; }
    virtual void fun() {
        std::cout << "B::fun" << std::endl;
    }
};

struct C : virtual public A
{
    C() { std::cout << "C"; }
    virtual ~C() { std::cout << "~C"; }
    virtual void fun() {
        std::cout << "C::fun" << std::endl;
    }
};

struct D : public B, public C
{
    D() { std::cout << "D"; }
    virtual ~D() { std::cout << "~D"; }
    virtual void fun() {
        std::cout << "D::fun" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    // create
    std::cout << "CREATE a ";
    A* a = new D;
    std::cout << std::endl << "CREATE b ";
    B* b = new D;
    std::cout << std::endl << "CREATE c ";
    C* c = new D;
    
    // call
    std::cout << std::endl << std::endl;
    
    a->fun();
    b->fun();
    c->fun();
    
    // destroy
    std::cout << std::endl << "DELETE a ";
    delete a;
    std::cout << std::endl << "DELETE b ";
    delete b;
    std::cout << std::endl << "DELETE c ";
    delete c;
    
    std::cout << std::endl;
    
    return 0;
}
