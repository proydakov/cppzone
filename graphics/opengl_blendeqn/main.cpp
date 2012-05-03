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
#include <fstream>
#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef _MSC_VER

#define GL_FUNC_ADD                       0x8006
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_BLEND_EQUATION                 0x8009
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B

typedef void (APIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;

#endif // _MSC_VER

const std::string COMMENT = "Keys are used to replace expressions mixing:\
                            \n\
                            'a' -> GL_FUNC_ADD,\n\
                            's' -> GL_FUNC_SUBTRACT\n\
                            'r' -> GL_FUNC_REVERS_SUBTRACT\n\
                            'n' -> GL_MIN\n\
                            'x' -> GL_MAX";

void init()
{
#ifdef _MSC_VER
    glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
    std::cout << "ADRESS : " << glBlendEquation << std::endl;
    if(glBlendEquation == NULL) {
        std::cout << "Function glBlendEquation not found.";
        exit(1);
    }
#endif // _MSC_VER

    std::cout << COMMENT << std::endl;
    glClearColor(1.0, 1.0, 0.0, 0.0);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glColor3d(0.0, 0.0, 1.0);
    glRectf(-0.5, -0.5, 0.5, 0.5);
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 'A':
        case 'a':
            glBlendEquation(GL_FUNC_ADD);
            break;

        case 'S':
        case 's':
            glBlendEquation(GL_FUNC_SUBTRACT);
            break;

        case 'r':
        case 'R':
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            break;

        case 'n':
        case 'N':
            glBlendEquation(GL_MIN);
            break;

        case 'x':
        case 'X':
            glBlendEquation(GL_MAX);
            break;
    
        case 27:
            exit(0);
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}