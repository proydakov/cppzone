#include <map>
#include <cstdint>
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

using map_t = std::map<std::int64_t, std::int64_t>;

#ifdef _MSC_VER

static_assert(sizeof(map_t::_Node) == (sizeof(map_t::value_type) + sizeof(void*) * 4), "Require fixed node size");

#endif // _MSC_VER

int main()
{
    std::cout << "value type: " << sizeof(map_t::value_type) << std::endl;

    map_t map;
    std::cout << "put" << std::endl;
    map.insert(std::make_pair(0, 0));

    return 0;
}
