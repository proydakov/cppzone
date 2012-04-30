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

#include <cmath>
#include <iostream>

#include <boost/thread.hpp>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "solar_system_info.h"

const int SPHERE_PRECESSION = 20;

static GLdouble g_mercuryYear = 0;
static GLdouble g_venusYear   = 0;
static GLdouble g_earthYear   = 0;
static GLdouble g_marsYear    = 0;
static GLdouble g_moonYear    = 0;

const int WORLD_SIZE_KOEF = 15;
const GLdouble ROTATION_VERTICAL_DELTA   = 0.05;
const GLdouble ROTATION_HORIZONTAL_DELTA = 0.05;

static GLdouble g_rotationVertical = 0.01;
static GLdouble g_rotationHorizontal = 0.75;

void init()
{
    glClearColor((GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0);
    glShadeModel(GL_FLAT);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    // draw sun
    glPushMatrix();
    {
        glColor3d(1.0, 1.0, 0.0);
        glutWireSphere(SUN_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    }
    glPopMatrix();
    
    glColor3d(0.0, 1.0, 0.0);

    // draw mercury
    glPushMatrix();
    {
        glColor3d(0.9, 0.9, 0.9);

        glRotated(g_mercuryYear, 0.0, 0.0, 1.0);
        glTranslated(MERCURY_ORBIT_RADIUS, 0.0, 0.0);
        glutWireSphere(MERCURY_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    }
    glPopMatrix();

    // draw venus
    glPushMatrix();
    {
        glColor3d(0.9, 0.9, 0.9);

        glRotated(g_venusYear, 0.0, 0.0, 1.0);
        glTranslated(VENUS_ORBIT_RADIUS, 0.0, 0.0);
        glutWireSphere(VENUS_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    }
    glPopMatrix();

    // draw earth
    glPushMatrix();
    {
        glColor3d(0.0, 0.0, 1.0);

        glRotated(g_earthYear, 0.0, 0.0, 1.0);
        glTranslated(EARTH_ORBIT_RADIUS, 0.0, 0.0);
        glutWireSphere(EARTH_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    
        glColor3d(0.9, 0.9, 0.9);

        glRotated(g_moonYear, 0.0, 0.0, 1.0);
        glTranslated(MOON_ORBIT_RADIUS, 0.0, 0.0);
        glutWireSphere(MOON_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    }
    glPopMatrix();

    // draw mars
    glPushMatrix();
    {
        glColor3d(1.0, 0.3, 0.0);

        glRotated(g_marsYear, 0.0, 0.0, 1.0);
        glTranslated(MARS_ORBIT_RADIUS, 0.0, 0.0);
        glutWireSphere(MARS_RADIUS, SPHERE_PRECESSION, SPHERE_PRECESSION);
    }
    glPopMatrix();

    glutSwapBuffers();

    // set look
    {
        GLdouble x = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * cos(g_rotationVertical);
        GLdouble y = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * sin(g_rotationVertical);
        GLdouble z = WORLD_SIZE_KOEF * cos(g_rotationHorizontal);

        glLoadIdentity();
        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    }
    glFlush();
}

void cycleFunction()
{
    g_mercuryYear = g_mercuryYear + MERCURY_YEAR_DELTA;
    g_venusYear = g_venusYear + VENUS_YEAR_DELTA;
    g_earthYear = g_earthYear + EARTH_YEAR_DELTA;
    g_marsYear = g_marsYear + MARS_YEAR_DELTA;

    g_moonYear = g_moonYear + MOON_YEAR_DELTA;
    
    if(g_mercuryYear > FULL_ANGLE)
        g_mercuryYear -= FULL_ANGLE;
    if(g_venusYear > FULL_ANGLE)
        g_venusYear -= FULL_ANGLE;
    if(g_earthYear > FULL_ANGLE)
        g_earthYear -= FULL_ANGLE;
    if(g_marsYear > FULL_ANGLE)
        g_marsYear -= FULL_ANGLE;

    if(g_moonYear > FULL_ANGLE)
        g_moonYear -= FULL_ANGLE;

    glutPostRedisplay();
    boost::this_thread::sleep(boost::posix_time::milliseconds(30)); 
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble) w / (GLdouble) h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 'A':
        case 'a':
            g_rotationVertical += ROTATION_VERTICAL_DELTA; 
            break;

        case 'D':
        case 'd':
            g_rotationVertical -= ROTATION_VERTICAL_DELTA;
            break;

        case 'W':
        case 'w':
            g_rotationHorizontal -= ROTATION_HORIZONTAL_DELTA;
            if(g_rotationHorizontal < 0)
                g_rotationHorizontal = 0.01;
            break;

        case 'S':
        case 's':
            g_rotationHorizontal += ROTATION_HORIZONTAL_DELTA;
            if(g_rotationHorizontal > 3.14)
                g_rotationHorizontal = 3.13;
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutIdleFunc(cycleFunction);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
