/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_OPERATIONS_WITH_STRINGS_H
#define I_OPERATIONS_WITH_STRINGS_H

#include <iostream>
#include <string.h>

typedef long index_type;

// ----------------------------- declaration ------------------------------- //

index_type substring_search_bf(const std::string& string, const std::string& substring);

index_type substring_search_aut(const std::string& string, const std::string& substring);

// ----------------------------- realization ------------------------------- //

index_type substring_search_bf(const std::string& string, const std::string& sub_string)
{
    index_type size = string.size() - sub_string.size() + 1;
    for(int i = 0; i < size; ++i) {
        if(memcmp(string.data() + i, sub_string.data(), sub_string.size()) == 0)
            return i;
    }
    return -1;
}

index_type substring_search_aut(const std::string& string, const std::string& substring)
{
    return -1;
}

#endif // I_OPERATIONS_WITH_STRINGS_H
