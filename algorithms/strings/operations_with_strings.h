/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
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
