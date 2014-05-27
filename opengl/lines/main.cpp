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

#include <algorithm>

#include <common/iglut.h>

typedef std::pair<GLdouble, GLdouble> point;

void drawLine(point p1, point p2)
{
    glBegin(GL_LINES);
    {
        glVertex2d(p1.first, p1.second);
        glVertex2d(p2.first, p2.second);
    }
    glEnd();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1.0, 1.0, 1.0);
    glEnable(GL_LINE_STIPPLE);

    // first row
    glLineStipple(1, 0x0101);
    drawLine(point(50.0, 125.0), point(150.0, 125.0));
    glLineStipple(1, 0x00FF);
    drawLine(point(150.0, 125.0), point(250.0, 125.0));
    glLineStipple(1, 0x1C47);
    drawLine(point(250.0, 125.0), point(350.0, 125.0));

    // second row
    glLineWidth(5.0);
    glLineStipple(1, 0x0101);
    drawLine(point(50.0, 100.0), point(150.0, 100.0));
    glLineStipple(1, 0x00FF);
    drawLine(point(150.0, 100.0), point(250.0, 100.0));
    glLineStipple(1, 0x1C47);
    drawLine(point(250.0, 100.0), point(350.0, 100.0));
    glLineWidth(1.0);

    // third row
    glLineStipple(1, 0x1C47);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < 7; ++i) {
        glVertex2d((GLfloat) 50.0 + ((GLfloat) i * (GLfloat) 50.0), (GLfloat) 75.0);
    }
    glEnd();

    // fourth row
    for(int i = 0; i < 6; ++i) {
        drawLine(point((GLfloat) 50.0 + (GLfloat) i * (GLfloat) 50.0, (GLfloat) 50.0),
            point((GLfloat) 50.0 + (GLfloat) (i + 1) * (GLfloat) 50.0, (GLfloat) 50.0));
    }

    // fifth row
    glLineStipple(5, 0x1C47);
    drawLine(point(50.0, 25.0), point(350.0, 25.0));

    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 150);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();

    return 0;
}
