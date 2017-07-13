#include <iostream>

class A
{
public:
    A(int val)
    {
        std::cout << "create A(): " << val << std::endl;
    }

    ~A()
    {
        std::cout << "release A()" << std::endl;
    }
};

class B
{
public:
    B(int val)
    {
        std::cout << "create B(): " << val << std::endl;
    }

    ~B()
    {
        std::cout << "release B()" << std::endl;
    }
};

class Meta
{
public:
    Meta() :
        b(1),
        a(2)
    {
    }

private:
    A a;
    B b;
};

int main()
{
    Meta meta;
    return 0;
}
