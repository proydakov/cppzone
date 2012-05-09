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

#ifndef I_PARABOLOID_H
#define I_PARABOLOID_H

#include <algorithm>
#include <object.h>

template <class P, class C>
void generate_paraboloid(object<P, C>& nobject, const C& color, GLfloat radius, unsigned precession)
{
    unsigned levels = precession / 25;
    unsigned point_on_level = precession / levels;

    GLfloat height = radius * 2;
    GLfloat height_delta = height / levels;
    GLfloat base = -radius;

    GLfloat radius_angle_delta = static_cast<GLfloat>(2 * M_PI / point_on_level);

    for(unsigned i = 0; i < levels; ++i) {
        for(unsigned j = 0; j < point_on_level; ++j) {

            GLfloat phi = j * radius_angle_delta;

            GLfloat z = height_delta * i;
            GLfloat cradius = 0.7f * sqrt(z);
            z += base;
            GLfloat x = cradius * cos(phi);
            GLfloat y = cradius * sin(phi);

            object<P, C>::point_type point(x, y, z);
            nobject.points.push_back(object<P, C>::vertex(point, color));
        }
    }
}

#endif // I_PARABOLOID_H