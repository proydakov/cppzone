#include <map>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

void* operator new(std::size_t sz)
{
    std::cout << "malloc, size = " << sz << std::endl;
    return std::malloc(sz);
}

void operator delete(void* ptr)
{
    std::cout << "free" << std::endl;
    std::free(ptr);
}

int safe_get(std::map<std::string, int>& map, const char* str)
{
    return map[str];
}

int main()
{
    std::ios::sync_with_stdio(false);

    std::map<std::string, int> map;
    std::cout << "put" << std::endl;

    safe_get(map, "big+str+in+heap+area+always");

    return 0;
}
