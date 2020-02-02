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

#ifndef I_CHAOS_H
#define I_CHAOS_H

#include <ctime>
#include <object.h>

template <class P, class C>
void generate_chaos(object<P, C>& nobject, const C& color, GLfloat radius, unsigned precession)
{
    srand((unsigned)time(NULL));

    for(unsigned i = 0; i < precession; ++i) {
        GLfloat phi = static_cast<GLfloat>(rand() % 360) / 360 * 2 * static_cast<GLfloat>(M_PI);
        GLfloat omega = static_cast<GLfloat>(rand() % 180) / 180 * static_cast<GLfloat>(M_PI);
        GLfloat cradius = static_cast<GLfloat>(rand() % 10) / 10 * radius;

        GLfloat x = cradius * sinf(phi) * cosf(omega);
        GLfloat y = cradius * sinf(phi) * sinf(omega);
        GLfloat z = cradius * cosf(phi);

        typename object<P, C>::point_type point(x, y, z);
        nobject.points.push_back(typename object<P, C>::vertex(point, color));
    }
}

#endif // I_CHAOS_H
