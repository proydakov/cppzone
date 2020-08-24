#include <cstdint>
#include <iostream>

#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

int main()
{
    std::uint32_t val{0};
    auto const res = _rdrand32_step(&val);
    if (res)
    {
        std::cout << "Value: " << val << std::endl;
    }
    else
    {
        std::cout << "Can't generate random number" << std::endl;
    }
    return 0;
}
