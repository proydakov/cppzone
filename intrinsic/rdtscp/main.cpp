#include <iostream>
#include <x86intrin.h>

int main()
{
    unsigned int aux{};
    auto const val = __rdtscp(&aux);
    std::cout << "val: " << val << " aux: " << aux << std::endl;
    return 0;
}
