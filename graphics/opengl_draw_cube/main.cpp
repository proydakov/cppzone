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

#include <GL/gl.h>
#include <GL/glu.h>

#ifndef _MSC_VER
#   include <unistd.h>
#else

#include <windows.h>

int usleep(int usec)
{
    Sleep(usec);
    return 0;
}

#endif // _MSC_VER

enum t_directions { direction_up, direction_right, direction_down, direction_left };

const double ROTATION_DELTA = 0.01;

static int g_width  = 500;
static int g_height = 500;

static int g_lastX = 0;
static int g_lastY = 0;

static double g_rotationVertical   = 1.0;
static double g_rotationHorizontal = 0.0;

t_directions getNextDirection()
{
    t_directions direction;

    double localX = g_lastX - g_width / 2;
    double localY = g_height / 2 - g_lastY;

    if(localX > 0 && localY > localX) {
        direction = direction_up;
    }
    else if(localX < 0 && localY > -localX) {
        direction = direction_up;
    }
    else if(localX > 0 && localY < -localX) {
        direction = direction_down;
    }
    else if(localX < 0 && localY < localX) {
        direction = direction_down;
    }
    else if(localX < 0) {
        direction = direction_left;
    }
    else
        direction = direction_right;

    return direction;
}

void threadFunctionUpdate()
{           
    t_directions direction = getNextDirection();

    switch(direction)
    {
    case direction_up:
        g_rotationHorizontal += ROTATION_DELTA;
        break;

    case direction_right:
        g_rotationVertical += ROTATION_DELTA;
        break;

    case direction_down:
        g_rotationHorizontal -= ROTATION_DELTA;
        break;

    case direction_left:
        g_rotationVertical -= ROTATION_DELTA;
        break;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glColor3f(1.0, 1.0, 1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glLoadMatrixf();
    //glMultMatrixf();
    
    double k = 3;
    double x = sin(g_rotationVertical) * k;
    double y = cos(g_rotationVertical) * k;
    double z = cos(g_rotationHorizontal) * k;

    std::cout << "X : " << x << " Y : " << y << " Z : " << z << std::endl;

    gluLookAt(x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    //glTranslated(0.0, 0.0, -5.0);
    //glScalef(1.0, 2.0, 1.0);

    glutWireCube(1.0);

    glPopMatrix();
    glutSwapBuffers();

    glFlush();
}

void reshape(int w, int h)
{
    g_width = w;
    g_height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    //gluPerspective(100.0, 1.0, 1.0, 20.0);
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            glutIdleFunc(threadFunctionUpdate);
        }
        else {
            glutIdleFunc(NULL);
        }

        g_lastX = x;
        g_lastY = y;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(g_width, g_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
