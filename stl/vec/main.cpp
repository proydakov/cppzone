#include <vector>
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
    std::vector<int> vec;
    std::cout << "created" << std::endl;

    std::cout << "size: " << vec.size() << std::endl;
    std::cout << "capacity: " << vec.capacity() << std::endl;

    std::cout << "push" << std::endl;

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "size: " << vec.size() << std::endl;
    std::cout << "capacity: " << vec.capacity() << std::endl;

    return 0;
}
