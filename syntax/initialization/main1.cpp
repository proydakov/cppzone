#include <iostream>

class A
{
public:
    A() :
        m1(1),
        m2(m3 - 1),
        m3(3)
    {
    }

    int m1;
    int m2;
    int m3;
};

int main()
{
    A obj;

    std::cout << "m1: " << obj.m1 << std::endl;
    std::cout << "m2: " << obj.m2 << std::endl;
    std::cout << "m3: " << obj.m3 << std::endl;

    return 0;
}
