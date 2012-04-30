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

const int WORLD_SIZE_KOEF = 15;
const GLdouble ROTATION_VERTICAL_DELTA   = 0.05;
const GLdouble ROTATION_HORIZONTAL_DELTA = 0.05;

static GLdouble g_rotationVertical   = -0.41;
static GLdouble g_rotationHorizontal =  0.75;

const GLdouble ANGLE_INCLINATION_BASE_MIN = -90.0;
const GLdouble ANGLE_INCLINATION_BASE_MAX = 90.0;
const GLdouble ANGLE_INCLINATION_SHOULDER_MIN = -90.0;
const GLdouble ANGLE_INCLINATION_SHOULDER_MAX = 90.0;
const GLdouble ANGLE_INCLINATION_FOREATM_MIN = -90.0;
const GLdouble ANGLE_INCLINATION_FOREATM_MAX = 90.0;

const GLdouble ANGLE_DELTA = 2;

static GLdouble g_angleRotationBase    = 0.0;
static GLdouble g_angleInclinationBase = -60.0;
static GLdouble g_angleInclinationShoulder = 60.0;
static GLdouble g_angleInclinationForearm  = 60.0;

void init()
{
    glClearColor((GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0);
    glShadeModel(GL_FLAT);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glutWireCube(2.0);
    glTranslated(0.0, 0.0, 1.0);

    glRotated(g_angleRotationBase, 0.0, 0.0, 1.0);
    glRotated(g_angleInclinationBase, 0.0, 1.0, 0.0);

    glTranslated(0.0, 0.0, 1.5);
    glPushMatrix();
    glScaled(1.0, 1.0, 3.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslated(0.0, 0.0, 1.5);
   
    glRotated(g_angleInclinationShoulder, 0.0, 1.0, 0.0);

    glTranslated(0.0, 0.0, 1.0);
    glPushMatrix();
    glScaled(1.0, 1.0, 2.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslated(0.0, 0.0, 1.0);

    glRotated(g_angleInclinationForearm, 0.0, 1.0, 0.0);

    glTranslated(0.0, 0.0, 1.0);
    glPushMatrix();
    glScaled(1.0, 1.0, 2.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslated(0.0, 0.0, 1.0);

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
    glutPostRedisplay();
    boost::this_thread::sleep(boost::posix_time::milliseconds(30)); 
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLdouble) w / (GLdouble) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    std::cout << "KEY : " << key << " | X : " << x << " | Y : " << y << std::endl;

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

        case '1':
            g_angleRotationBase -= ANGLE_DELTA;
            break;

        case '2':
            g_angleRotationBase += ANGLE_DELTA;
            break;

        case '3':
            g_angleInclinationBase -= ANGLE_DELTA;
            if(g_angleInclinationBase < ANGLE_INCLINATION_BASE_MIN)
                g_angleInclinationBase = ANGLE_INCLINATION_BASE_MIN;
            break;

        case '4':
            g_angleInclinationBase += ANGLE_DELTA;
            if(g_angleInclinationBase > ANGLE_INCLINATION_BASE_MAX)
                g_angleInclinationBase = ANGLE_INCLINATION_BASE_MAX;
            break;

        case '5':
            g_angleInclinationShoulder -= ANGLE_DELTA;
            if(g_angleInclinationShoulder < ANGLE_INCLINATION_SHOULDER_MIN)
                g_angleInclinationShoulder = ANGLE_INCLINATION_SHOULDER_MIN;
            break;

        case '6':
            g_angleInclinationShoulder += ANGLE_DELTA;
            if(g_angleInclinationShoulder > ANGLE_INCLINATION_SHOULDER_MAX)
                g_angleInclinationShoulder = ANGLE_INCLINATION_SHOULDER_MAX;
            break;

        case '7':
            g_angleInclinationForearm -= ANGLE_DELTA;
            if(g_angleInclinationForearm < ANGLE_INCLINATION_FOREATM_MIN)
                g_angleInclinationForearm = ANGLE_INCLINATION_FOREATM_MIN;
            break;

        case '8':
            g_angleInclinationForearm += ANGLE_DELTA;
            if(g_angleInclinationForearm > ANGLE_INCLINATION_FOREATM_MAX)
                g_angleInclinationForearm = ANGLE_INCLINATION_FOREATM_MAX;
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
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
