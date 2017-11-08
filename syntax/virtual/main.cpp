#include <iostream>

class A
{
public:
    A() : data_a(get())
    {
        std::cout << "build A" << std::endl;
        call();
    }

    virtual void call()
    {
        std::cout << "A::call" << std::endl;
    }

    virtual int get()
    {
        std::cout << "A::get" << std::endl;
        return 1;
    }

    int data_a;
};

class B : public A
{
public:
    B() : data_b(get())
    {
        std::cout << "build B" << std::endl;
        A::call();
        call();
    }

    void call() override
    {
        std::cout << "B::call" << std::endl;
    }

    int get() override
    {
        std::cout << "B::get" << std::endl;
        return 2;
    }

    int data_b;
};

int main()
{
    std::cout << "create B" << std::endl;
    B b;

    std::cout << "call method" << std::endl;
    b.call();

    return 0;
}
