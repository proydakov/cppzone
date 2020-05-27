#include <iostream>

#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

int main()
{
    unsigned int aux{};
    auto const val = __rdtscp(&aux);
    std::cout << "val: " << val << " aux: " << aux << std::endl;
    return 0;
}
