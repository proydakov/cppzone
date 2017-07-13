#include <iostream>

class A
{
public:
    A() : data_(1)
    {
        std::cout << "create A()" << std::endl;
    }

    ~A()
    {
        std::cout << "release A()" << std::endl;
    }

    A(const A& other)
    {
        std::cout << "create copy A()" << std::endl;
        data_ = other.data_;
    }

    A& operator=(const A& other)
    {
        std::cout << "operator= A()" << std::endl;
        data_ = other.data_;
        return *this;
    }

private:
    int data_;
};

int main()
{
    A a1;
    A a2 = a1;

    return 0;
}
