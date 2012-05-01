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

#include <boost/thread.hpp>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

const GLdouble SPIN_DELTA = 3.0;

static GLdouble g_spin0 = 0.0;
static GLdouble g_spin1 = 0.0;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);

    GLfloat light0_color[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_color[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_color);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_color);
}

void display()
{
    GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glPushMatrix();
    glRotated(g_spin0, 1.0, 0.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glTranslated(0.0, 0.0, 2.0);
    glDisable(GL_LIGHTING);
    glColor3d(0.0, 1.0, 1.0);
    glutWireCube(0.1);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glRotated(g_spin1, 0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glTranslated(0.0, 0.0, 2.0);
    glDisable(GL_LIGHTING);
    glColor3d(0.0, 1.0, 1.0);
    glutWireCube(0.1);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glutSolidTorus(0.275, 0.85, 20, 50);
    glPopMatrix();

    glutSwapBuffers();

    glLoadIdentity();
    gluLookAt(0, 5, 5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLdouble) w / (GLdouble) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void cycle()
{
    g_spin0 += SPIN_DELTA;
    if(g_spin0 > 360)
        g_spin0 -= 360;

    g_spin1 += SPIN_DELTA * 7 / 10;
    if(g_spin1 > 360)
        g_spin1 -= 360;

    glutPostRedisplay();
    boost::this_thread::sleep(boost::posix_time::milliseconds(30)); 
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(cycle);
    glutMainLoop();

    return 0;
}