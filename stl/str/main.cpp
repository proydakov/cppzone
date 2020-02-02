#include <string>
#include <iostream>

void* operator new(std::size_t sz)
{
    std::cout << "malloc, size = " << sz << std::endl;
    return std::malloc(sz);
}

void operator delete(void* ptr) noexcept
{
    std::cout << "free" << std::endl;
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    std::cout << "free[" << size << ']' << std::endl;
    std::free(ptr);
}

int main()
{
    std::string str;

    std::cout << "size: " << str.size() << std::endl;
    std::cout << "capacity: " << str.capacity() << std::endl;

    std::cout << "push" << std::endl;

    for(char i = 0; i < 31; i++) {
        str.push_back(static_cast<char>('0' + i));
    }

    std::cout << "size: " << str.size() << std::endl;
    std::cout << "capacity: " << str.capacity() << std::endl;

    return 0;
}
