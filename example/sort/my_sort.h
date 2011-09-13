/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_MY_SORT_H
#define I_MY_SORT_H

#include <vector>
#include <algorithm>

typedef long index_type;

// ----------------------------- declaration ------------------------------- //

template<typename type>
void bubble_sort(std::vector<type>& data);

template<typename type>
void shaker_sort(std::vector<type>& data);

template<typename type>
void selection_sort(std::vector<type>& data);

template<typename type>
void insertion_sort(std::vector<type>& data);

template<typename type>
void shell_sort(std::vector<type>& data);

template<typename type>
void quick_sort(std::vector<type>& data);

template<typename type>
void quick_sort(std::vector<type> &data, index_type begin, index_type end);

// ----------------------------- realization ------------------------------- //

template<typename type>
void bubble_sort(std::vector<type>& data)
{
    bool key = true;
    index_type size = data.size() - 1;
    for(index_type i = size; i >= 0; --i) {
        key = false;
        for(index_type j = 0; j < i; ++j) {
            if(data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                key = true;
            }
        }
        if(!key) {
            break;
        }
    }
}

template<typename type>
void shaker_sort(std::vector<type>& data)
{
    index_type size = data.size() - 1;
    index_type begin = 0;
    index_type end = size;
    index_type index = 0;
    while(begin < end) {
        for(index = begin; index < end; ++index) {
            if(data[index] > data[index + 1]) {
                std::swap(data[index], data[index + 1]);
            }
        }
        end = index - 1;

        for(index = end; index > begin; --index) {
            if(data[index] < data[index - 1]) {
                std::swap(data[index], data[index - 1]);
            }
        }
        begin = index + 1;
    }
}

template<typename type>
void selection_sort(std::vector<type>& data)
{
    index_type min_element_index;
    index_type size = data.size();
    for(index_type i = 0; i < size; ++i) {
        min_element_index = i;
        for(index_type j = i + 1; j < size; ++j) {
            if(data[j] < data[min_element_index]) {
                min_element_index = j;
            }
        }
        std::swap(data[i], data[min_element_index]);
    }
}

template<typename type>
void insertion_sort(std::vector<type>& data)
{
    index_type size = data.size();
    for(index_type i = 1; i < size; ++i) {
        type element = data[i];
        index_type j = i - 1;
        while(j >= 0 && data[j] > element) {
            data[j + 1] = data[j];
            --j;
        }
        data[j + 1] = element;
    }
}

template<typename type, class index>
void shell_sort_increment(std::vector<type>& data, index size)
{
    index p1 = 1;
    index p2 = 1;
    index p3 = 1;
    index s = -1;
    do {
        if (++s % 2) {
            data.push_back(8*p1 - 6*p2 + 1);
        }
        else {
            data.push_back(9*p1 - 9*p3 + 1);
            p2 *= 2;
            p3 *= 2;
        }
        p1 *= 2;
    }
    while(3 * data[s] < size);
}

template<typename type>
void shell_sort(std::vector<type>& data)
{
    index_type size = data.size();
    std::vector<index_type> increment_seq;
    shell_sort_increment(increment_seq, size);
    index_type increment_index = increment_seq.size() - 1;
    while(increment_index >= 0) {
        for(index_type i = 1; i < size; ++i) {
            type element = data[i];
            index_type j = i - increment_seq[increment_index];
            while(j >= 0 && data[j] > element ) {
                data[j + increment_seq[increment_index]] = data[j];
                j -= increment_seq[increment_index];
            }
            data[j + increment_seq[increment_index]] = element;
        }
        --increment_index;
    }
}

template<typename type>
void quick_sort(std::vector<type>& data)
{
    quick_sort(data, 0, data.size() - 1);
}

template<class type>
void quick_sort(std::vector<type> &data, index_type begin, index_type end)
{
    index_type i = begin;
    index_type j = end;
    type medium = data[(i + j) >> 1];
    while(i <= j) {
        while(data[i] < medium)
            ++i;
        while(data[j] > medium)
            --j;
        if(i <= j) {
            std::swap(data[i], data[j]);
            ++i;
            --j;
        }
    }
    if(i < end)
        quick_sort(data, i, end);
    if(j > begin)
        quick_sort(data, begin, j);
}

#endif // I_MY_SORT_H
