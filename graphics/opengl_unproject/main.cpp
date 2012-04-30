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

#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void init()
{
    glClearColor((GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0, (GLclampf) 0.0);
    glShadeModel(GL_FLAT);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble) w / (GLdouble) h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(state != GLUT_DOWN)
        return;

    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;
    GLdouble wx, wy, wz;

    if(button == GLUT_LEFT_BUTTON) {
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetDoublev(GL_PROJECTION_MATRIX, mvmatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

        realy = viewport[3] - (GLint) y - 1;
        std::cout << "Coordinates of the cursor : " << x << ", " << realy << std::endl;

        for(GLdouble z = 0.0; z < 2; z += 1.0) {
            gluUnProject((GLdouble) x, (GLdouble) realy, z, 
                mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

            std::cout << "World coordinates at z = " << z 
                << " | WX : " << wx << " | WY : " << wy << " | WZ " << wz << std::endl; 
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv)
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
