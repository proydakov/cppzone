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

#ifndef I_TORUS_H
#define I_TORUS_H

#include <cmath>
#include <object.h>

template <class P, class C>
void generate_torus(object<P, C>& nobject, const C& color, GLfloat big_radius, GLfloat small_radius, unsigned precession)
{
    unsigned circles_count = precession * 1 / 25;
    unsigned points_on_circle = precession / circles_count;

    GLfloat big_radius_angle_delta = static_cast<GLfloat>(2 * M_PI / circles_count);
    GLfloat small_radius_angle_delta = static_cast<GLfloat>(2 * M_PI / points_on_circle);

    for(unsigned i = 0; i < circles_count; ++i) {

        GLfloat phi = GLfloat(i) * big_radius_angle_delta;
        GLfloat xGlobal = 0;
        GLfloat yGlobal = 0;

        for(unsigned j = 0; j < points_on_circle; ++j) {

            GLfloat omega = GLfloat(j) * small_radius_angle_delta;

            GLfloat xLocal = small_radius * cosf(omega) - big_radius;
            GLfloat yLocal = 0;
            GLfloat zLocal = small_radius * sinf(omega);

            GLfloat xLocalRotated = xLocal * cosf(phi) - yLocal * sinf(phi);
            GLfloat yLocalRotated = xLocal * sinf(phi) + yLocal * cosf(phi);

            GLfloat xReal = xGlobal + xLocalRotated;
            GLfloat yReal = yGlobal + yLocalRotated;
            GLfloat zReal = zLocal;

            typename object<P, C>::point_type point(xReal, yReal, zReal);

            nobject.points.push_back(typename object<P, C>::vertex(point, color));
        }
    }

    int t = 0;
    (void)t;
}

#endif // I_TORUS_H
