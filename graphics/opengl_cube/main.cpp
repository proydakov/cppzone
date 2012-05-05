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

#include <GL/glut.h>

const int CYCLE_TIME = 30;

const int WORLD_SIZE_KOEF = 3;
const GLdouble ROTATION_VERTICAL_DELTA   = 0.05;
const GLdouble ROTATION_HORIZONTAL_DELTA = 0.05;

static GLdouble g_rotationVertical = 0.01;
static GLdouble g_rotationHorizontal = 0.75;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glColor3d(1.0, 1.0, 1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glLoadMatrixf();
    //glMultMatrixf();
    
    // set look
    {
        GLdouble x = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * cos(g_rotationVertical);
        GLdouble y = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * sin(g_rotationVertical);
        GLdouble z = WORLD_SIZE_KOEF * cos(g_rotationHorizontal);

        glLoadIdentity();
        gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    glutWireCube(1.0);

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    //gluPerspective(100.0, 1.0, 1.0, 20.0);
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

void cycle(int value)
{
    glutPostRedisplay();
    glutTimerFunc(CYCLE_TIME, cycle, value);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(CYCLE_TIME, cycle, 0);
    glutMainLoop();

    return 0;
}
