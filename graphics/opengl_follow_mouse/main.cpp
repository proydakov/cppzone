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

#include <list>
#include <iostream>
#include <algorithm>

#include <GL/glut.h>

#include <GL/gl.h>
#include <GL/glu.h>

typedef std::pair<GLfloat, GLfloat> point;
typedef std::list<point> pointList;

pointList g_pointList;

void drawPolyline(pointList list)
{
    if(list.size() < 2)
        return;

    glBegin(GL_LINE_STRIP);
    {
        pointList::const_iterator endIt = list.end();
        for(pointList::const_iterator it = list.begin(); it != endIt; ++it) {
            glVertex2f(it->first, it->second);
        }
    }
    glEnd();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x1C47);
    glColor3f(1.0, 1.0, 1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawPolyline(g_pointList);

    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, (GLdouble) h, 0.0);
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(state != GLUT_DOWN)
        return;

    if(button == GLUT_LEFT_BUTTON) {
        g_pointList.push_back(point((GLfloat) x, (GLfloat) y));
        std::cout << "X : " << x << " | Y : " << y << std::endl;
    }

    if(button == GLUT_RIGHT_BUTTON)
        g_pointList.clear();

    std::cout << "DATA SIZE : " << g_pointList.size() << std::endl;

#ifdef __GNUC__
     display();
#endif // __GNUC__
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
