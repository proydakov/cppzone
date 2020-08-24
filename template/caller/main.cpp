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

#include <list>
#include <string>
#include <iostream>

class A
{
public:
    void fun1() {
        std::cout << "A::fun1()" << std::endl;
    }

    void fun2(int i) {
        std::cout << "A::fun2(" << i << ")" << std::endl;
    }

    void fun3(const std::string& str) {
        std::cout << "A::fun3(" << str << ")" << std::endl;
    }
};

class B
{
public:
    void method1() {
        std::cout << "B::method1()" << std::endl;
    }

    void method2(double d, const std::string& str) {
        std::cout << "B::method2(" << d << ", " << str << ")" << std::endl;
    }
};

struct i_caller
{
    virtual void run() = 0;
};

template <class T, class F>
class caller_base
{
public:
    caller_base(T* p_object, F p_function)
        : m_p_object  (p_object  )
        , m_p_function(p_function)
    {
    }

protected:
    T* m_p_object;
    F  m_p_function;
};

template <class T, class F>
class caller_p0: public i_caller, public caller_base<T, F>
{
public:
    caller_p0(T* p_object, F p_function) :
        caller_base<T, F>(p_object, p_function) {
    }

private:
    virtual void run() {
        (caller_base<T, F>::m_p_object->*caller_base<T, F>::m_p_function)();
    }
};

template <class T, class F, class P1>
class caller_p1: public i_caller, public caller_base<T, F>
{
public:
    caller_p1(T* p_object, F p_function, const P1& p1) :
        caller_base<T, F>(p_object, p_function),
        m_param1(p1)
    {
    }

private:
    virtual void run() {
        (caller_base<T, F>::m_p_object->*caller_base<T, F>::m_p_function)(m_param1);
    }

    P1  m_param1;
};

template <class T, class F, class P1, class P2>
class caller_p2: public i_caller, public caller_base<T, F>
{
public:
    caller_p2(T* p_object, F p_function, const P1& p1, const P2& p2) :
        caller_base<T, F>(p_object, p_function),
        m_param1(p1),
        m_param2(p2)
    {
    }

private:
    virtual void run() {
        (caller_base<T, F>::m_p_object->*caller_base<T, F>::m_p_function)(m_param1, m_param2);
    }

    P1  m_param1;
    P2  m_param2;
};

typedef void (A::*pointer1)();
typedef void (A::*pointer2)(int);
typedef void (B::*pointer3)(double, const std::string&);
typedef void (A::*pointer4)(const std::string&);

int main()
{
    std::list<i_caller*> list;

    A a;
    B b;
    caller_p0<A, pointer1>      a_fun1(&a, &A::fun1);
    caller_p1<A, pointer2, int> a_fun2(&a, &A::fun2, 10);
    caller_p1<A, pointer2, int> a_fun3(&a, &A::fun2, 20);
    caller_p2<B, pointer3, double, std::string> a_fun4(&b, &B::method2, 11.1, "abc");
    caller_p1<A, pointer4, std::string> a_fun5(&a, &A::fun3, "bde");

    list.push_back(&a_fun1);
    list.push_back(&a_fun2);
    list.push_back(&a_fun3);
    list.push_back(&a_fun4);
    list.push_back(&a_fun5);

    auto it = list.begin();
    while(it != list.end()) {
        (*it)->run();
        ++it;
    }
    return 0;
}
