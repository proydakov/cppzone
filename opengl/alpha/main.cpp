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

const std::string COMMENT = "Press any key to change the order of drawing objects.\nPress Esc for exit...";
const GLdouble OBJECT_SIDE = 1;

static int g_leftFirst = GL_TRUE;

void info()
{
    std::cout << COMMENT << std::endl;
}

void init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void drawTriangle(GLdouble side, GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    glBegin(GL_POLYGON);
    {
        glColor4d(red, green, blue, alpha);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(side, 0.0, 0.0);
        glVertex3d(0.0, side, 0.0);
    }
    glEnd();
}

void drawLeftTriangle()
{
    glPushMatrix();
    glTranslated(-OBJECT_SIDE / 4, 0, 0);
    drawTriangle(OBJECT_SIDE, 1.0, 1.0, 0.0, 0.75);
    glPopMatrix();
}

void drawRightTriangle()
{
    glPushMatrix();
    glTranslated(OBJECT_SIDE / 4, 0, 0);
    drawTriangle(OBJECT_SIDE, 0.0, 1.0, 1.0, 0.75);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if(g_leftFirst) {
        drawLeftTriangle();
        drawRightTriangle();
    }
    else {
        drawRightTriangle();
        drawLeftTriangle();
    }

    glFlush();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 27:
            exit(0);
            break;

        default:
            g_leftFirst = !g_leftFirst;
            glutPostRedisplay();
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-OBJECT_SIDE / 2, OBJECT_SIDE * 3 / 2, -OBJECT_SIDE / 2, OBJECT_SIDE * 3 / 2);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    info();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}