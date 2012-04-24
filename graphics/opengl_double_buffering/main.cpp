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

#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _MSC_VER

#include <windows.h>

int usleep(int usec)
{
    Sleep(usec);
    return 0;
}

#endif // _MSC_VER

static GLfloat g_spin = 0.0;
static bool g_rotate_stae = false;

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
    glRotatef(g_spin, 0.0, 0.0, 1.0);
    glRectf(-25.0, -25.0, 25.0, 25.0);
    glPopMatrix();
    glutSwapBuffers();

    usleep(1);
}

void spinDisplay()
{
    g_spin += 2.0;
    if(g_spin > 360.0)
        g_spin -= 360.0;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(state != GLUT_DOWN)
        return;

    if(button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
        if(g_rotate_stae)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(spinDisplay);
        g_rotate_stae = !g_rotate_stae;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
