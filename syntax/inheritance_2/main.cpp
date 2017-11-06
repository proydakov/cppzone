#include <iostream>

struct A
{
    A()
    {
        std::cout << "A" << std::endl;
    }

    ~A()
    {
        std::cout << "~A" << std::endl;
    }
};

struct B
{
    B()
    {
        std::cout << "B" << std::endl;
    }

    ~B()
    {
        std::cout << "~B" << std::endl;
    }
};

struct C : public A, public B
{
    C()
    {
        std::cout << "C" << std::endl;
    }

    ~C()
    {
        std::cout << "~C" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    C obj;

    return 0;
}
