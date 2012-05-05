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

template<class T>
T pow_simple(T base, T factor);

template<class T>
T pow_advance(T base, T factor);

template<class T>
T fibonacci_recursive(T n);

template<class T>
T fibonacci_matrix(T n);

template<class T>
T factorial_recursive(T n);

template<class T>
T factorial_cyclic(T n);

template<class T>
T factorial_stirling(T n);

template<class T>
T sum_divided_by_the_factorial(T n);

template<class T>
T lowest_common_divisor(T a, T b);

template<class T>
T lowest_common_divisor_euclid(T a, T b);

template<class T>
T lowest_common_divisor_modified_euclid(T a, T b);

// ----------------------------- realization ------------------------------- //

template<class T>
T pow_simple(T base, T factor)
{
    T res = 1;
    for(int i = 0; i < factor; ++i)
        res *= base;
    return res;
}

template<class T>
T pow_advance(T base, T factor)
{
    if(factor == 1)
        return base;
    else {
        T c = base * base;
        T ans = pow_advance(c, factor / 2);
        if(factor % 2)
            return base * ans;
        else
            return ans;
    }
}

template<class T>
T fibonacci_recursive(T n)
{
    if(n == 0) {
        return 0;
    }
    else if(n == 1) {
        return 1;
    }
    else {
        T start = 0;
        start += fibonacci_recursive<T>(n - 1) + fibonacci_recursive<T>(n - 2);
        return start;
    }
}

template<class T>
T fibonacci_matrix(T n)
{
    if(n == 0) {
        return 0;
    }
    else if(n == 1) {
        return 1;
    }
    else {
        boost::numeric::ublas::matrix<T> matrix(2, 2);
        matrix(0, 0) = 1;
        matrix(0, 1) = 1;
        matrix(1, 0) = 1;
        matrix(1, 1) = 0;
        
        boost::numeric::ublas::matrix<T> res_matrix(matrix);
        for(int i = 0; i < n; ++i)
            res_matrix = prod(res_matrix, matrix);
        
        boost::numeric::ublas::vector<T> vector(2);
        vector[0] = 0;
        vector[1] = 1;
        
        boost::numeric::ublas::vector<T> res = prod(res_matrix, vector);
        
        return res[1];
    }
}

template<class T>
T factorial_recursive(T n)
{
    if(n == 0) {
        return 1;
    }
    else {
        T start = static_cast<T>(n);
        start *= factorial_recursive<T>(n - 1);
        return start;
    }
}

template<class T>
T factorial_cyclic(T n)
{
    if(n == 0) {
        return 1;
    }
    else {
        T start = 1;
        for(int i = 2; i <= n; ++i) {
            start *= i;
        }
        return start;
    }
}

template<class T>
T factorial_stirling(T n)
{
    return sqrt(2 * 3.14159265 * n) * pow(n / 2.718281828, n) * 
            (1 + 1/(12 * n) + 1/(128 * pow(n, 2)) - 139/(51840 * pow(n, 3)));
}

template<class T>
T sum_divided_by_the_factorial(T n)
{
    if(n == 0) {
        return 1;
    }
    else {
        T factorial = 1;
        T res = 1;
        for(int i = 2; i <= n; ++i) {
            factorial *= i;
            res += 1 / factorial;
        }
        return res;
    }
}

template<class T>
T largest_common_divisor(T a, T b)
{
    T k = std::min(a, b);
    while(k > 0) {
        if(!(a % k) && !(b % k))
            break;
        --k;
    }
    return k;
}

template<class T>
T largest_common_divisor_euclid(T a, T b)
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

template<class T>
T largest_common_divisor_modified_euclid(T a, T b)
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
