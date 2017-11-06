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

void process(std::string&& src)
{
    std::cout << "enter_process" << std::endl;
    std::string local(std::move(src));
    std::cout << "local: " << local << std::endl;
    std::cout << "leave_process" << std::endl;
}

int main()
{
    std::cout << "enter_main" << std::endl;

    std::string str("main+string+in+heap+area+long+size");

    std::cout << "main1: " << str << std::endl;
    process(std::move(str));
    std::cout << "main2: " << str << std::endl;

    std::cout << "leave_main" << std::endl;

    return 0;
}
