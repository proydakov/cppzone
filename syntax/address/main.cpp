#include <memory>
#include <iostream>

template<class T>
struct Ptr {
    T* pad; // add pad to show difference between 'this' and 'data'
    T* data;
    Ptr(T* arg) : pad(nullptr), data(arg) 
    {
        std::cout << "ptr this = " << this << std::endl;
    }
 
    ~Ptr() { delete data; }
    T** operator&() { return &data; }
};

int main()
{
    auto* ptr = new Ptr<int>(nullptr);
    std::cout << "ptr raw  = " << ptr << std::endl;
    std::cout << "ptr addr = " << &*ptr << std::endl;

    return 0;
}
