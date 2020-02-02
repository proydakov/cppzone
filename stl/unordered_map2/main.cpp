#include <vector>
#include <iostream>
#include <unordered_map>

void* operator new(std::size_t sz)
{
    void* ptr = std::malloc(sz);
    std::cout << "malloc, size = " << sz << " ptr: " << ptr << std::endl;
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::cout << "free ptr: " << ptr << std::endl;
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    std::cout << "free ptr=" << ptr << " size=" << size << std::endl;
    std::free(ptr);
}

int main()
{
    using map_t = std::unordered_map<int, int>;

    map_t map{
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 4, 4 },
        { 5, 5 }
    };

    std::cout << "bcount: " << map.bucket_count() << std::endl;

    std::cout << "reserve 1024" << std::endl;

    map.reserve(1024);
    std::cout << "bcount: " << map.bucket_count() << std::endl;

    map.reserve(1024);
    std::cout << "bcount: " << map.bucket_count() << std::endl;

    return 0;
}
