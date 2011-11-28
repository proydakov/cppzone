/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_OPERATIONS_WITH_ARRAYS_H
#define I_OPERATIONS_WITH_ARRAYS_H

#include <vector>

typedef long index_type;

// ----------------------------- declaration ------------------------------- //

template<typename data_type>
void merge(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res);

template<typename data_type>
void intersection(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res);


template<typename data_type>
void diminution(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res);

template<typename data_type>
data_type binary_search(const std::vector<data_type>& data, const data_type& a);

// ----------------------------- realization ------------------------------- //

template<typename data_type>
void merge(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res)
{
    res.clear();
    
    index_type index_a = 0;
    index_type index_b = 0;
    
    while(index_a < a.size() && index_b < b.size()) {
        if(a[index_a] < b[index_b]) {
            res.push_back(a[index_a]);
            ++index_a;
        }
        else {
            res.push_back(b[index_b]);
            ++index_b;
        }
    }
    
    while(index_a < a.size()) {
        res.push_back(a[index_a]);
        ++index_a;
    }
    while(index_b < b.size()) {
        res.push_back(b[index_b]);
        ++index_b;
    }
}

template<typename data_type>
void intersection(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res)
{
    res.clear();
    
    index_type index_a = 0;
    index_type index_b = 0;
    
    while(index_a < a.size() && b.size()) {
        if(a[index_a] > b[index_b]) {
            ++index_b;
        }
        else if(a[index_a] < b[index_b]) {
            ++index_a;
        }
        else {
            res.push_back(a[index_a]);
            ++index_a;
            ++index_b;
        }
    }
}

template<typename data_type>
void subtraction(const std::vector<data_type>& a, const std::vector<data_type>& b, std::vector<data_type>& res)
{
    res.clear();
    
    index_type index_a = 0;
    index_type index_b = 0;
    
    index_type size_a = a.size();
    index_type size_b = b.size();
    
    while(index_a < size_a && index_b < size_b) {
        if(a[index_a] == b[index_b]) {
            ++index_a;
            ++index_b;
        }
        else if(a[index_a] > b[index_b]) {
            ++index_b;
        }
        else {
            res.push_back(a[index_a]);
            ++index_a;
        }
    }
    while(index_a < size_a) {
        res.push_back(a[index_a]);
        ++index_a;
    }
}

template<typename data_type>
data_type binary_search(const std::vector<data_type>& data, const data_type& a)
{
    index_type begin = 0;
    index_type end = data.size() - 1;

    index_type middle;
    while(begin <= end) {
        middle = (end + begin) / 2;
        if(data[middle] > a) {
            end = middle - 1;
        }
        else if(data[middle] < a) {
            begin = middle + 1;
        }
        else {
            return middle;
        }
    }
    return -1;
}

#endif // I_OPERATIONS_WITH_ARRAYS_H
