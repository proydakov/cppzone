/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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
