#include <iostream>
#include <x86intrin.h>

int main()
{
    auto const val = __rdtsc();
    std::cout << "val: " << val << std::endl;
    return 0;
}
