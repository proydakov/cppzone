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

#ifndef I_SPIRAL_H
#define I_SPIRAL_H

#include <cmath>
#include <object.h>

template <class P, class C>
void generate_spiral(object<P, C>& nobject, const C& color, GLfloat radius, unsigned precession)
{
    GLfloat height = radius * 2;
    GLfloat height_delta = height / precession;
    GLfloat base = -radius;
    GLfloat angle_delta = 0.1f;

    GLfloat compression = 1.0f / precession;

    for(unsigned i = 0; i < precession; ++i) {
        
        GLfloat phi = angle_delta * i;

        GLfloat x = radius * cos(phi) * i * compression;
        GLfloat y = radius * sin(phi) * i * compression;
        GLfloat z = base + height_delta * i;

        typename object<P, C>::point_type point(x, y, z);
        nobject.points.push_back(typename object<P, C>::vertex(point, color));
    }
}

#endif // I_SPIRAL_H
