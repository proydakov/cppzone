#include "file.h"

#include <iostream>

size_t trace1()
{
    std::cout << "ADDR FROM FILE1: " << (void*)(&A::g_map) << std::endl;

    return A::g_map.size();
}
