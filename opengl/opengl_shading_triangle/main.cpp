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

void info()
{
    int red_bits, green_bits, blue_bits, alpha_bits, index_bits;

    glGetIntegerv(GL_RED_BITS, &red_bits);
    glGetIntegerv(GL_GREEN_BITS, &green_bits);
    glGetIntegerv(GL_BLUE_BITS, &blue_bits);
    glGetIntegerv(GL_ALPHA_BITS, &alpha_bits);
    glGetIntegerv(GL_INDEX_BITS, &index_bits);

    std::cout << "GL_RED_BITS : " << red_bits << std::endl;
    std::cout << "GL_GREEN_BITS : " << green_bits << std::endl;
    std::cout << "GL_BLUE_BITS : " << blue_bits << std::endl;
    std::cout << "GL_ALPHA_BITS : " << alpha_bits << std::endl;
    std::cout << "GL_INDEX_BITS : " << index_bits << std::endl;
}

void drawTriangle()
{
    glBegin(GL_TRIANGLES);
    {
        glColor3d(1.0, 0.0, 0.0);
        glVertex2d(5.0, 5.0);
        glColor3d(0.0, 1.0, 0.0);
        glVertex2d(25.0, 5.0);
        glColor3d(0.0, 0.0, 1.0);
        glVertex2d(5.0, 25.0);
    }
    glEnd();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    info();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    drawTriangle();
    glutSwapBuffers();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w <= h)
        gluOrtho2D(0.0, 30.0, 0.0, 30.0 * (GLdouble) h / (GLdouble) w);
    else
        gluOrtho2D(0.0, 30.0 * (GLdouble) w / (GLdouble) h, 0.0, 30.0);
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
    glutMainLoop();

    return 0;
}
