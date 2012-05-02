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

#include <vector>
#include <boost/thread.hpp>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

const GLdouble CLEAR_COLOR[] = { 0.0, 0.0, 0.0 };
const GLdouble DRAW_COLOR[]  = { 1.0, 1.0, 1.0 };

const int CYCLE_TIME = 1250;

static size_t g_index = 0;
static std::vector<int> g_cursors;

void init()
{
    glClearColor((GLclampf) CLEAR_COLOR[0], (GLclampf) CLEAR_COLOR[1],
        (GLclampf) CLEAR_COLOR[2], (GLclampf) 0.0);
    glShadeModel(GL_FLAT);

    g_cursors.push_back(GLUT_CURSOR_RIGHT_ARROW);
    g_cursors.push_back(GLUT_CURSOR_LEFT_ARROW);
    g_cursors.push_back(GLUT_CURSOR_INFO);
    g_cursors.push_back(GLUT_CURSOR_DESTROY);
    g_cursors.push_back(GLUT_CURSOR_HELP);
    g_cursors.push_back(GLUT_CURSOR_CYCLE);
    g_cursors.push_back(GLUT_CURSOR_SPRAY);
    g_cursors.push_back(GLUT_CURSOR_WAIT);
    g_cursors.push_back(GLUT_CURSOR_TEXT);
    g_cursors.push_back(GLUT_CURSOR_CROSSHAIR);
    g_cursors.push_back(GLUT_CURSOR_UP_DOWN);
    g_cursors.push_back(GLUT_CURSOR_LEFT_RIGHT);
    g_cursors.push_back(GLUT_CURSOR_TOP_SIDE);
    g_cursors.push_back(GLUT_CURSOR_BOTTOM_SIDE);
    g_cursors.push_back(GLUT_CURSOR_LEFT_SIDE);
    g_cursors.push_back(GLUT_CURSOR_RIGHT_SIDE);
    g_cursors.push_back(GLUT_CURSOR_TOP_LEFT_CORNER);
    g_cursors.push_back(GLUT_CURSOR_TOP_RIGHT_CORNER);
    g_cursors.push_back(GLUT_CURSOR_BOTTOM_RIGHT_CORNER);
    g_cursors.push_back(GLUT_CURSOR_BOTTOM_LEFT_CORNER);
    g_cursors.push_back(GLUT_CURSOR_INHERIT);
    g_cursors.push_back(GLUT_CURSOR_NONE);
    g_cursors.push_back(GLUT_CURSOR_FULL_CROSSHAIR);
}

void display()
{
    if(g_index % 2)
        glColor3dv(CLEAR_COLOR);
    else
        glColor3dv(DRAW_COLOR);

    glClear(GL_COLOR_BUFFER_BIT);
 
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_POLYGON);
    {
        glVertex3d(0.25, 0.25, 0.0);
        glVertex3d(0.75, 0.25, 0.0);
        glVertex3d(0.75, 0.75, 0.0);
        glVertex3d(0.25, 0.75, 0.0);
    }
    glEnd();

    glutSwapBuffers();
 
    // set cursor method
    glutSetCursor(g_cursors[g_index]);
    
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void cycle()
{
    g_index = (g_index + 1) % g_cursors.size();

    glutPostRedisplay();
    boost::this_thread::sleep(boost::posix_time::milliseconds(CYCLE_TIME)); 
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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
