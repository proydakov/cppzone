#pragma once

#include <string>
#include <unordered_map>

namespace A
{

inline const std::unordered_map<int, std::string> g_map = { {1, "one"}, {2, "two"} };

}

size_t trace1();
size_t trace2();

