/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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

#ifndef I_OBJECT_H
#define I_OBJECT_H

#include <cmath>
#include <vector>

#ifndef M_PI
#   define M_PI     3.14159265358979323846
#endif // M_PI

template <class T>
struct point3d
{
    typedef T data_type;

    point3d(T x, T y, T z) :
        m_x(x), m_y(y), m_z(z) 
        {}

        bool compare(const point3d<T>& src, T precession) {
            return fabs(src.m_x - m_x) < precession && 
                fabs(src.m_y - m_y) < precession &&
                fabs(src.m_z - m_z) < precession;
        }

    T m_x;
    T m_y;
    T m_z;
};

template <class T>
struct color_rgba
{
    color_rgba() :
        m_red(0),
        m_green(0),
        m_blue(0),
        m_alpha(0)
        {
        }

    color_rgba(T red, T green, T blue, T alpha) :
        m_red(red),
        m_green(green),
        m_blue(blue),
        m_alpha(alpha)
        {
        }

    T m_red;
    T m_green;
    T m_blue;
    T m_alpha;
};

template <class P, class C>
struct object
{
    typedef P        point_type;
    typedef C        color_type;

    struct vertex
    {
        vertex(P point, C color) :
            m_point(point),
            m_color(color)
            {
            }

        P m_point;
        C m_color;
    };

    std::vector<vertex> points;
};

#endif // I_OBJECT_H