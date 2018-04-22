#include <iostream>

int main()
{
    char const* c = "hello world";

    std::cout << "c1: " << c << std::endl;

    char* bad_c = (char*)(c);
    bad_c[0] = 'b';

    std::cout << "c2: " << c << std::endl;

    return 0;
}
