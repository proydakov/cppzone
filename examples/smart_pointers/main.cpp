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
#include <vector>
#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

class A
{
public:
    A(const std::string& name) :
        m_name(name)
    {
        std::cout << "Creating class A with name = " << m_name << "\n";
    }
    virtual ~A() { std::cout << "Destructing class A with name =" << m_name << "\n"; }

    std::string get_name() { return m_name; };

private:
    std::string m_name;
};

class B : public A
{
public:
    B(int b, const std::string& name) :
        A(name),
        m_b(b)
    {
        std::cout << "Creating class B with name = " << get_name() << "  and  m_b = " << m_b << "\n";
    }
    virtual ~B() { std::cout << "Destructing class B with name = " << get_name() << "  and  m_b = " << m_b << "\n"; }

private:
    int m_b;
};

typedef boost::shared_ptr<A> a_prt;

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    boost::uuids::random_generator random_gen;

    {
        a_prt p(new B(2, boost::uuids::to_string(random_gen())));
        (void)p;
    }

    std::list<a_prt> list;
    std::vector<a_prt> vector;

    for(int i = 7; i < 9; ++i) {
        a_prt my_prt(new B(i, boost::uuids::to_string(random_gen())));

        list.push_back(my_prt);
        vector.push_back(my_prt);

        my_prt.reset();
    }

    list.clear();

    vector.push_back(a_prt(new B(77, boost::uuids::to_string(random_gen()))));

    return 0;
}
