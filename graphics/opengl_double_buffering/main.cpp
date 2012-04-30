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

#include <boost/thread.hpp>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

static GLdouble g_spin = 0.0;
static bool g_rotate_stae = false;
static bool g_fill_state = false;

void init()
{
    glClearColor((GLclampf) 0.1, (GLclampf) 0.0, (GLclampf) 0.7, (GLclampf) 0.0);
    glShadeModel(GL_FLAT);
    glColor3d(0.0, 1.0, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotated(g_spin, 0.0, 0.0, 1.0);

    glPolygonMode(GL_FRONT, GL_FILL);
    if(g_fill_state) {
        glPolygonMode(GL_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_BACK, GL_FILL);
    }

    glBegin(GL_POLYGON);
    {
        glVertex2d(0.0, 0.0);
        glVertex2d(0.0, 30.0);
        glVertex2d(40.0, 30.0);
        glVertex2d(60.0, 15);
        glVertex2d(40.0, 0.0);
    }
    glEnd();
    glPopMatrix();

    glutSwapBuffers();

    boost::this_thread::sleep(boost::posix_time::milliseconds(5));
}

void spinDisplay()
{
    g_spin += (GLfloat) 0.1;
    if(g_spin > 360.0)
        g_spin -= 360.0;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-(GLsizei) w / 2, (GLsizei) w / 2, -(GLsizei) h / 2, (GLsizei) h / 2);
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(state != GLUT_DOWN)
        return;

    if(button == GLUT_LEFT_BUTTON) {
        if(g_rotate_stae)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(spinDisplay);
        g_rotate_stae = !g_rotate_stae;
    }
    else if(button == GLUT_RIGHT_BUTTON) {
        g_fill_state = !g_fill_state;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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
