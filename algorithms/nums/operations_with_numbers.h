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

#ifndef I_OPERATIONS_WITH_NUMBERS_H
#define I_OPERATIONS_WITH_NUMBERS_H

#include <cmath>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

// ----------------------------- declaration ------------------------------- //

template<typename data_type, typename index_type>
data_type fibonacci_recursive(index_type n);

template<typename data_type, typename index_type>
data_type fibonacci_matrix(index_type n);

template<typename data_type, typename index_type>
data_type factorial_recursive(index_type n);

template<typename data_type, typename index_type>
data_type factorial_cyclic(index_type n);

template<typename data_type, typename index_type>
data_type factorial_stirling(index_type n);

template<typename data_type, typename index_type>
data_type sum_divided_by_the_factorial(index_type n);

template<typename data_type>
data_type lowest_common_divisor(data_type a, data_type b);

template<typename data_type>
data_type lowest_common_divisor_euclid(data_type a, data_type b);

template<typename data_type>
data_type lowest_common_divisor_modified_euclid(data_type a, data_type b);

// ----------------------------- realization ------------------------------- //

template<typename data_type, typename index_type>
data_type fibonacci_recursive(index_type n)
{
    if(n == 0) {
        return 0;
    }
    else if(n == 1) {
        return 1;
    }
    else {
        data_type start = 0;
        start += fibonacci_recursive<data_type, index_type>(n - 1) + fibonacci_recursive<data_type, index_type>(n - 2);
        return start;
    }
}

template<typename data_type, typename index_type>
data_type fibonacci_matrix(index_type n)
{
    if(n == 0) {
        return 0;
    }
    else if(n == 1) {
        return 1;
    }
    else {
        boost::numeric::ublas::matrix<data_type> matrix(2, 2);
        matrix(0, 0) = 1;
        matrix(0, 1) = 1;
        matrix(1, 0) = 1;
        matrix(1, 1) = 0;
        
        boost::numeric::ublas::matrix<data_type> res_matrix(matrix);
        for(int i = 0; i < n; ++i)
            res_matrix = prod(res_matrix, matrix);
        
        boost::numeric::ublas::vector<data_type> vector(2);
        vector[0] = 0;
        vector[1] = 1;
        
        boost::numeric::ublas::vector<data_type> res = prod(res_matrix, vector);
        
        return res[1];
    }
}

template<typename data_type, typename index_type>
data_type factorial_recursive(index_type n)
{
    if(n == 0) {
        return 1;
    }
    else {
        data_type start = static_cast<data_type>(n);
        start *= factorial_recursive<data_type, index_type>(n - 1);
        return start;
    }
}

template<typename data_type, typename index_type>
data_type factorial_cyclic(index_type n)
{
    if(n == 0) {
        return 1;
    }
    else {
        data_type start = 1;
        for(int i = 2; i <= n; ++i) {
            start *= i;
        }
        return start;
    }
}

template<typename data_type, typename index_type>
data_type factorial_stirling(index_type n)
{
    return sqrt(2 * 3.14159265 * n) * pow(n / 2.718281828, n) * 
            (1 + 1/(12 * n) + 1/(128 * pow(n, 2)) - 139/(51840 * pow(n, 3)));
}

template<typename data_type, typename index_type>
data_type sum_divided_by_the_factorial(index_type n)
{
    if(n == 0) {
        return 1;
    }
    else {
        data_type factorial = 1;
        data_type res = 1;
        for(int i = 2; i <= n; ++i) {
            factorial *= i;
            res += 1/factorial;
        }
        return res;
    }
}

template<typename data_type>
data_type largest_common_divisor(data_type a, data_type b)
{
    data_type k = std::min(a, b);
    while(k > 0) {
        if(!(a % k) && !(b % k))
            break;
        --k;
    }
    return k;
}

template<typename data_type>
data_type largest_common_divisor_euclid(data_type a, data_type b)
{
    while(a > 0 && b > 0) {
        if(a > b) {
            a -= b;
        }
        else {
            b -= a;
        }
    }
    if(a > 0) {
        return a;
    }
    return b;
}

template<typename data_type>
data_type largest_common_divisor_modified_euclid(data_type a, data_type b)
{
    while(a > 0 && b > 0) {
        if(a > b) {
            a %= b;
        }
        else {
            b %= a;
        }
    }
    if(a > 0) {
        return a;
    }
    return b;  
}

#endif // I_OPERATIONS_WITH_NUMBERS_H
