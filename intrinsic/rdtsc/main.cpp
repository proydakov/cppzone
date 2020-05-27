#include <iostream>

#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

int main()
{
    auto const val = __rdtsc();
    std::cout << "val: " << val << std::endl;
    return 0;
}
