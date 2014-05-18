/*
 *  Copyright (c) 2011-2014 Evgeny Proydakov <lord.tiran@gmail.com>
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

#ifndef I_MY_SORT_H
#define I_MY_SORT_H

#include <set>
#include <vector>
#include <algorithm>

// ----------------------------- declaration ------------------------------- //

template<class type>
void selection_sort(std::vector<type>& data);

template<class type>
void bubble_sort(std::vector<type>& data);

template<class type>
void shaker_sort(std::vector<type>& data);

template<class type>
void insertion_sort(std::vector<type>& data);

template<class type>
void shell_sort(std::vector<type>& data);

template<class type>
void merge_sort(std::vector<type>& data);

template<class type>
void merge_sort(std::vector<type>& data, size_t begin, size_t end, std::vector<type>& buffer);

template<class type>
void heap_sort(std::vector<type>& data);

template<class type>
void quick_sort(std::vector<type>& data);

template<class type>
void quick_sort(std::vector<type> &data, size_t begin, size_t end);

// ----------------------------- realization ------------------------------- //

template<class type>
void bubble_sort(std::vector<type>& data)
{
    bool key = true;
    size_t size = data.size() - 1;
    for(size_t i = size; i >= 0; --i) {
        key = false;
        for(size_t j = 0; j < i; ++j) {
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

template<class type>
void shaker_sort(std::vector<type>& data)
{
    size_t size = data.size() - 1;
    size_t begin = 0;
    size_t end = size;
    size_t index = 0;
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

template<class type>
void selection_sort(std::vector<type>& data)
{
    size_t min_element_index;
    size_t size = data.size();
    for(size_t i = 0; i < size; ++i) {
        min_element_index = i;
        for(size_t j = i + 1; j < size; ++j) {
            if(data[j] < data[min_element_index]) {
                min_element_index = j;
            }
        }
        std::swap(data[i], data[min_element_index]);
    }
}

template<class type>
void insertion_sort(std::vector<type>& data)
{
    size_t size = data.size();
    for(size_t i = 1; i < size; ++i) {
        type element = data[i];
        size_t j = i - 1;
        for(;j >= 0 && data[j] > element; --j) {
            data[j + 1] = data[j];
        }
        data[j + 1] = element;
    }
}

template<class type>
void merge(std::vector<type>& data, size_t begin, size_t middle, size_t end, std::vector<type>& buffer)
{
    size_t pos1 = begin;
    size_t pos2 = middle + 1;
    size_t pos3 = 0;

    while(pos1 <= middle && pos2 <= end) {
        if (data[pos1] < data[pos2]) {
            buffer[pos3] = data[pos1];
            ++pos3;
            ++pos1;
        }
        else {
            buffer[pos3] = data[pos2];
            ++pos2;
            ++pos3;
        }
    }
    while(pos2 <= end) {
        buffer[pos3] = data[pos2];
        ++pos3;
        ++pos2;
    }
    while(pos1 <= middle) {
        buffer[pos3] = data[pos1];
        ++pos3;
        ++pos1;
    }
    for(size_t i = 0; i < end - begin + 1; i++) {
        data[begin + i] = buffer[i];
    }
}

template<class type>
void merge_sort(std::vector<type>& data)
{
    std::vector<type> buffer(data.size(), 0);
    merge_sort(data, 0, data.size() - 1, buffer);
}

template<class type>
void merge_sort(std::vector<type>& data, size_t begin, size_t end, std::vector<type>& buffer)
{
    if(begin < end) {
        size_t middle = (begin + end) / 2;
        merge_sort(data, begin, middle, buffer);
        merge_sort(data, middle + 1, end, buffer);
        merge(data, begin, middle, end, buffer);
    }
}

template<class type>
void heap_sort(std::vector<type>& data)
{
    std::make_heap(data.begin(), data.end());
    std::sort_heap(data.begin(), data.end());
}

template<class type>
void quick_sort(std::vector<type>& data)
{
    quick_sort(data, 0, data.size() - 1);
}

template<class type>
void quick_sort(std::vector<type> &data, size_t begin, size_t end)
{
    size_t i = begin;
    size_t j = end;

    type medium = data[(i + j) / 2];
    while(i <= j) {
        while(data[i] < medium) {
            ++i;
        }
        while(data[j] > medium) {
            --j;
        }
        if(i <= j) {
            std::swap(data[i], data[j]);
            ++i;
            --j;
        }
    }
    if(i < end) {
        quick_sort(data, i, end);
    }
    if(j > begin) {
        quick_sort(data, begin, j);
    }
}

#endif // I_MY_SORT_H
