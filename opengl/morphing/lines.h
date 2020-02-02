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

#ifndef I_LINES_H
#define I_LINES_H

#include <cmath>
#include <object.h>

template <class P, class C>
void generate_lines(object<P, C>& nobject, const C& color, GLfloat radius, unsigned precession)
{
    GLfloat height = radius * 2;
    GLfloat base = 0 - height / 2;

    unsigned lines_count = precession * 1 / 25;
    unsigned points_on_line = precession / lines_count;

    GLfloat big_radius_angle_delta = static_cast<GLfloat>(2 * M_PI / lines_count);
    GLfloat height_delta = height / GLfloat(points_on_line);

    for(unsigned i = 0; i < points_on_line; ++i) {
        for(unsigned j = 0; j < lines_count; ++j) {
            GLfloat phi = big_radius_angle_delta * GLfloat(j);

            GLfloat x =  radius * cosf(phi);
            GLfloat y = -radius * sinf(phi);
            GLfloat z = base + height_delta * GLfloat(i);

            typename object<P, C>::point_type point(x, y, z);

            nobject.points.push_back(typename object<P, C>::vertex(point, color));
        }
    }
}

#endif // I_LINES_H
