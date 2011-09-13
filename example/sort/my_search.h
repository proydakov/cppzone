/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_MY_SEARCH_H
#define I_MY_SEARCH_H

#include <vector>

typedef long index_type;

template<class type>
index_type binary_seach(std::vector<type>& data, type element)
{
    if(data.size() == 0) {
        return -1;
    }

}

#endif // I_MY_SEARCH_H
