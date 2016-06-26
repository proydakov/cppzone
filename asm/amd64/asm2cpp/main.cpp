#include <iostream>

extern "C" int PassingParameters(int input);

int main()
{
    std::cout << "Get FROM ASM: " << PassingParameters(100) << std::endl;
    return 0;
}
