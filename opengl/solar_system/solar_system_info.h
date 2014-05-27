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

#include <common/igl.h>

#ifndef I_SOLAR_SYSTEM_INFO_H
#define I_SOLAR_SYSTEM_INFO_H

const GLdouble FULL_ANGLE = 360.0;

const GLdouble MERCURY_YEAR_DELTA = FULL_ANGLE / 88;
const GLdouble VENUS_YEAR_DELTA   = FULL_ANGLE / 225;
const GLdouble EARTH_YEAR_DELTA   = FULL_ANGLE / 365;
const GLdouble MARS_YEAR_DELTA    = FULL_ANGLE / 780;

const GLdouble MOON_YEAR_DELTA    = FULL_ANGLE / 28;

const GLdouble SPACE_OBJECT_SIZE_SCALE = 50000;

const GLdouble SUN_RADIUS     = SPACE_OBJECT_SIZE_SCALE / SPACE_OBJECT_SIZE_SCALE;
const GLdouble MERCURY_RADIUS = 2439 / SPACE_OBJECT_SIZE_SCALE;
const GLdouble VENUS_RADIUS   = 6051 / SPACE_OBJECT_SIZE_SCALE;
const GLdouble EARTH_RADIUS   = 6371 / SPACE_OBJECT_SIZE_SCALE;
const GLdouble MARS_RADIUS    = 3376 / SPACE_OBJECT_SIZE_SCALE;

const GLdouble MOON_RADIUS    = 1736 / SPACE_OBJECT_SIZE_SCALE;

const GLdouble SPACE_OBJECT_LENGTH_SCALE = SUN_RADIUS * 0.5 * 60000000;

const GLdouble MERCURY_ORBIT_RADIUS = 57909068.0  / SPACE_OBJECT_LENGTH_SCALE;
const GLdouble VENUS_ORBIT_RADIUS   = 108208930.0 / SPACE_OBJECT_LENGTH_SCALE;
const GLdouble EARTH_ORBIT_RADIUS   = 149597887.5 / SPACE_OBJECT_LENGTH_SCALE;
const GLdouble MARS_ORBIT_RADIUS    = 224396831.3 / SPACE_OBJECT_LENGTH_SCALE;

const GLdouble MOON_ORBIT_RADIUS    = 384399 * 25 / SPACE_OBJECT_LENGTH_SCALE;

#endif // I_SOLAR_SYSTEM_INFO_H