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

#ifndef I_VECTOR
#define I_VECTOR

#include <cmath>
#include <string.h>
#include <exception>

template<class T>
class vector
{
public:
    typedef unsigned long size_type;
    
public:
    vector();
    vector(size_type size, const T& value = T());
    vector(const vector<T>& src);
    vector& operator==(const vector<T>& src);
    ~vector();
    
    size_type size() const;
    size_type capacity() const;
    size_type max_size() const;
    bool empty() const;
    void resize(size_type size, const T& value = T());
    void reserve(size_type size);
    
    const T& operator[](size_type n) const;
    T& operator[](size_type n);
    const T& at(size_type n) const;
    T& at(size_type n);
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    
    void push_back(const T& value);
    void pop_back();
    void erase(size_type n);
    void swap(vector<T>& vect);
    void clear();
    
private:
    static const double RATE_EXPANSION  = 1.4;
    static const double VECTOR_MAX_SIZE = 4294967295;
    
private:
    inline void check_limits(size_type n);
    void copy(const vector<T>& src);
    
private:
    T* m_begin;
    T* m_end;
    T* m_reserv;
};

#ifndef NULL_PTR
#   define NULL_PTR 0
#endif // NULL_PTR

template<class T>
vector<T>::vector() :
    m_begin(NULL_PTR),
    m_end(NULL_PTR),
    m_reserv(NULL_PTR)
{
}

template<class T>
vector<T>::vector(vector<T>::size_type size, const T& value)
{
    check_limits(size);
    
    double new_size = size * RATE_EXPANSION;
    if(new_size > VECTOR_MAX_SIZE)
        new_size = VECTOR_MAX_SIZE;
    
    m_begin = new T[(size_type)new_size];
    m_end = m_begin + size;
    m_reserv = m_begin + (size_type)new_size;
    
    for(size_type i = 0; i < size; ++i) {
        *(m_begin + i) = value;   
    }
}

template<class T>
vector<T>::vector(const vector<T>& src)
{
    copy(src);
}

template<class T>
vector<T>& vector<T>::operator==(const vector<T>& src)
{
    if(this == &src)
        return *this;
    
    clear();
    copy(src);
    return *this;
}

template<class T>
vector<T>::~vector()
{
    delete [] m_begin;
}

template<class T>
typename vector<T>::size_type vector<T>::size() const
{
    return m_end - m_begin;
}

template<class T>
typename vector<T>::size_type vector<T>::capacity() const
{
    return m_reserv - m_begin;
}

template<class T>
typename vector<T>::size_type vector<T>::max_size() const
{
    return VECTOR_MAX_SIZE;
}

template<class T>
bool vector<T>::empty() const
{
    return m_begin == m_end;
}

template<class T>
void vector<T>::resize(size_type size, const T& value)
{
    size_type now_size = m_end - m_begin;
    if(size < now_size)
        m_end = m_begin + size;
    else {
        reserve(size);
        for(size_type i = now_size; i < size; ++i) {
            *(m_begin + i) = value;
        }
    }
}

template<class T>
void vector<T>::reserve(vector<T>::size_type size)
{
    if(m_begin + size < m_reserv)
        return;
    
    size_type last_size = m_end - m_begin;
    T* m_del = m_begin;
    double new_size = size * RATE_EXPANSION;
    m_begin = new T[(size_type)new_size];
    m_end = m_begin + size;
    m_reserv = m_begin + (size_type)new_size;
    for(size_type i = 0; i < last_size; ++i)
        *(m_begin + i) = *(m_del + i);
    delete [] m_del;
}

template<class T>
const T& vector<T>::at(size_type n) const
{
    return at(n);
}

template<class T>
T& vector<T>::at(size_type n)
{
    check_limits(n);
    return operator [](n);
}

template<class T>
const T& vector<T>::operator[](size_type n) const
{
    return operator [](n);
}

template<class T>
T& vector<T>::operator[](size_type n)
{
    return *(m_begin + n);
}

template<class T>
const T& vector<T>::front() const
{
    return front();
}

template<class T>
T& vector<T>::front()
{
    return operator [](0);
}

template<class T>
const T& vector<T>::back() const
{
    return back();
}

template<class T>
T& vector<T>::back()
{
    return *(m_end - 1);
}

template<class T>
void vector<T>::push_back(const T& value)
{
    if(m_end == m_reserv)
        reserve((m_reserv - m_begin));
    *m_end = value;
    ++m_end;
}

template<class T>
void vector<T>::pop_back()
{
    if(m_end > m_begin)
        --m_end;
}

template<class T>
void vector<T>::erase(size_type n)
{
    check_limits(n);
    for(int i = n; i < m_end - m_begin - 1; ++i) {
        *(m_begin + n) = *(m_begin + n + 1);
    }
}

template<class T>
void vector<T>::swap(vector<T>& vect)
{
    std::swap(vect.m_begin, m_begin);
    std::swap(vect.m_end, m_end);
    std::swap(vect.m_reserv, m_reserv);
}

template<class T>
void vector<T>::clear()
{
    m_end = m_begin;
}

template<class T>
void vector<T>::check_limits(size_type n)
{
    if(n > VECTOR_MAX_SIZE || n < 0)
        throw std::exception();
}

template<class T>
void vector<T>::copy(const vector<T>& src)
{
    size_type new_size = src.m_reserv - src.m_begin;
    size_type size = src.m_end - src.m_begin;
    m_begin = new T[new_size];
    m_end = m_begin + (size);
    m_reserv = m_begin + new_size;
    memcpy(src.m_begin, m_begin, size);
}

#endif // I_VECTOR
