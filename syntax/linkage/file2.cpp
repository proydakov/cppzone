#include "file.h"

#include <iostream>

size_t trace2()
{
    std::cout << "ADDR FROM FILE2: " << (void*)(&A::g_map) << std::endl;

    return A::g_map.size();
}
