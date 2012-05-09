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

#ifndef I_CUBE_H
#define I_CUBE_H

#include <object.h>

unsigned calc_cube_point(unsigned n)
{
    if(n == 1)
        return 1;
    if(n == 2)
        return 8;
    return 6 * n * n - 12 * n + 8;
}

template <class P, class C>
void generate_cube(object<P, C>& nobject, const C& color, GLfloat size, unsigned precession)
{
    unsigned side = 0;
    unsigned generated = 0;

    for(unsigned i = 1; generated < precession; ++i) {
        generated = calc_cube_point(i);
        side = i - 1;
    }

    int delta = precession - calc_cube_point(side);

    GLfloat maxX = size / 2;
    GLfloat maxY = size / 2;
    GLfloat maxZ = size / 2;

    if(side % 2) {
        maxX -= size / (side * 2);
        maxY -= size / (side * 2);
        maxZ -= size / (side * 2);
    }

    unsigned last = side - 1;

    for(unsigned ix = 0; ix < side; ++ix) {
        for(unsigned iy = 0; iy < side; ++iy) {
            for(unsigned iz = 0; iz < side; ++iz) {

                GLfloat x = maxX - size * ix / side;
                GLfloat y = maxY - size * iy / side;
                GLfloat z = maxZ - size * iz / side;

                if(ix == 0 || ix == last ||
                    iy == 0 || iy == last ||
                    iz == 0 || iz == last) {

                    object<P, C>::point_type point(x, y, z);

                    nobject.points.push_back(object<P, C>::vertex(point, color));

                    if(delta > 0) {
                        delta--;
                        nobject.points.push_back(object<P, C>::vertex(point, color));
                    }
                }
            }
        }
    }

    while(delta > 0) {
        delta--;
        nobject.points.push_back(nobject.points[nobject.points.size() - 1]);
    }
}

#endif // I_CUBE_H