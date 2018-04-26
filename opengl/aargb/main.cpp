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

#include <string>
#include <iostream>

#include <common/iglut.h>

const GLdouble WORLD_SIZE_KOEF = 1.0;
const GLdouble ROTATION_DELTA  = 0.5;
const int CYCLE_TIME = 20;
GLdouble g_rotationAngle = 0;

bool g_AntiAliasing = true;

const std::string COMMENT = "Press 'a' for enable / disable AntiAliasing. Press ESC for exit.";

void init()
{
    GLfloat values[2];

    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    std::cout << "GL_LINE_WIDTH_GRANULARITY : " << values[0] << std::endl;

    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    std::cout << "GL_LINE_WIDTH_RANGE : " << values[0] << std::endl;

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void initAntiAliasing()
{
    if(g_AntiAliasing)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else
        glBlendFunc(GL_ONE, GL_ONE);
}

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    initAntiAliasing();

    glColor3d(0.0, 1.0, 0.0);
    glPushMatrix();
    glRotated(-g_rotationAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    {
        glVertex2d(-0.5, 0.5);
        glVertex2d(0.5, -0.5);
    }
    glEnd();
    glPopMatrix();

    glColor3d(0.0, 0.0, 1.0);
    glPushMatrix();
    glRotated(g_rotationAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    {
        glVertex2d(0.5, 0.5);
        glVertex2d(-0.5, -0.5);
    }
    glEnd();

    glFlush();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w <= h)
        glOrtho(-WORLD_SIZE_KOEF, WORLD_SIZE_KOEF, -WORLD_SIZE_KOEF * (GLdouble) h / (GLdouble) w,
        WORLD_SIZE_KOEF * (GLdouble) h / (GLdouble) w, -10.0, 10.0);
    else
        glOrtho(-WORLD_SIZE_KOEF * (GLdouble) w / (GLdouble) h, WORLD_SIZE_KOEF * (GLdouble) w / (GLdouble) h,
        -WORLD_SIZE_KOEF, WORLD_SIZE_KOEF, -10.0, 10.0);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 'A':
        case 'a':
            g_AntiAliasing = !g_AntiAliasing;
            break;

        case 27:
            exit(0);
            break;

        default:
            break;
    }
}

void cycle(int value)
{
    g_rotationAngle += ROTATION_DELTA;
    if(g_rotationAngle > 360.0)
        g_rotationAngle -= 360.0;
    glutPostRedisplay();
    glutTimerFunc(CYCLE_TIME, cycle, value);
}

int main(int argc, char** argv)
{
    info();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
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
