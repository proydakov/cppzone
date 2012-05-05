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

#ifdef _MSC_VER

#   define GL_MULTISAMPLE_ARB                0x809D
#   define GL_SAMPLE_BUFFERS_ARB             0x80A8
#   define GL_SAMPLES_ARB                    0x80A9

#endif // _MSC_VER

const GLdouble WORLD_SIZE_KOEF = 2;

int g_bgtoggle = 1;

void init()
{
    GLint buf;
    GLint sbuf;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glGetIntegerv(GL_SAMPLE_BUFFERS_ARB, &buf);
    std::cout << "Number of buffers of samples : " << buf << std::endl;

    glGetIntegerv(GL_SAMPLES_ARB, &sbuf);
    std::cout << "Number of samples equal : " << sbuf << std::endl;

    glNewList(1, GL_COMPILE);
    {
        int size = 19;
        for(int i = 0; i < size; ++i) {
            glPushMatrix();
            glRotated(360.0 * (GLdouble) (i / size), 0.0, 0.0, 1.0);
            glLineWidth((GLfloat)((i % 3) + 1.0));

            glColor3d(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
            {
                glVertex2d(0.25, 0.05);
                glVertex2d(0.9, 0.2);
            }
            glEnd();

            glColor3d(0.0, 1.0, 1.0);
            glBegin(GL_TRIANGLES);
            {
                glVertex2d(0.25, 0.0);
                glVertex2d(0.9, 0.0);
                glVertex2d(0.875, 0.1);
            }
            glEnd();

            glPopMatrix();
        }
    }
    glEndList();

    glNewList(2, GL_COMPILE);
    {
        glColor3d(1.0, 0.5, 0.0);
     
        int size = 16;
        glBegin(GL_POLYGON);
        {
            for(int i = 0; i < size; ++i) {
                for(int j = 0; j < size; ++j) {
                    if(!((i + j) % 2)) {
                        glVertex2d(-2.0 + i * 0.25, -2.0 + j * 0.25);
                        glVertex2d(-2.0 + i * 0.25, -1.75 + j * 0.25);
                        glVertex2d(-1.75 + i * 0.25, -1.75 + j * 0.25);
                        glVertex2d(-1.75 + i * 0.25, -2.0 + j * 0.25);
                    }
                }
            }
        }
        glEnd();
    }
    glEndList();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(g_bgtoggle)
        glCallList(2);

    glEnable(GL_MULTISAMPLE_ARB);
    glPushMatrix();
    glTranslated(-1.0, 0.0, 0.0);
    glCallList(1);
    glPopMatrix();

    glDisable(GL_MULTISAMPLE_ARB);
    glPushMatrix();
    glTranslated(1.0, 0.0, 0.0);
    glCallList(1);
    glPopMatrix();

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
        case 'b':
        case 'B':
            g_bgtoggle = !g_bgtoggle;
            glutPostRedisplay();
            break;

        case 27:
            exit(0);
            break;

        default:
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}