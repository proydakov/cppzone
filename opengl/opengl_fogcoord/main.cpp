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

#include <GL/glut.h>

#ifdef _MSC_VER
#   define GL_FOG_COORDINATE_SOURCE          0x8450
#   define GL_FOG_COORDINATE                 0x8451
#   define GL_FRAGMENT_DEPTH                 0x8452
#else
#   include <GL/glx.h>
#endif // _MSC_VER

///////////////////////////////////////////////////////////////////////////////

const std::string COMMENT = 
"Press 'c' or 'd' for select GL_FOG_COORDINATE_SOURCE.\n\
Press 'b' or 'f' for select TRANSLATED.\n\
Press '1' - '6' for control point and fog depth.\n\
Press ESC for exit.";

///////////////////////////////////////////////////////////////////////////////

typedef void (APIENTRY * PFNGLFOGCOORDDPROC) (GLdouble coord);
PFNGLFOGCOORDDPROC glFogCoordd = NULL;

const GLdouble WORLD_SIZE_KOEF = 2.5;
const GLdouble ITER_DELTA      = 0.25;

GLdouble g_f1 = 0;
GLdouble g_f2 = 0;
GLdouble g_f3 = 0;

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void init()
{
#ifdef _MSC_VER
    glFogCoordd = (PFNGLFOGCOORDDPROC)wglGetProcAddress("glFogCoordd");
#else
     glFogCoordd = (PFNGLFOGCOORDDPROC)glXGetProcAddress((const GLubyte*)"glFogCoordd");
#endif // _MSC_VER

    std::cout << "ADRESS : " << glFogCoordd << std::endl;
    if(glFogCoordd == NULL) {
        std::cout << "Function glFogCoordd not found.";
        exit(1);
    }

    GLfloat fogColor[] = { 0.0f, 0.25f, 0.25f, 1.0f };
    g_f1 = 1;
    g_f2 = 5;
    g_f3 = 10;

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.25f);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogi(GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE);

    glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glColor3d(1.0, 0.75, 0.0);
    glBegin(GL_TRIANGLES);
    {
        glFogCoordd(g_f1);
        glVertex3d(2.0, -2.0, 0.0);
        glFogCoordd(g_f2);
        glVertex3d(-2.0, 0.0, -5.0);
        glFogCoordd(g_f3);
        glVertex3d(0.0, 2.0, -10.0);
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
        case 'D':
        case 'd':
            glFogi(GL_FOG_COORDINATE_SOURCE, GL_FRAGMENT_DEPTH);
            std::cout << "SET : GL_FOG_COORDINATE_SOURCE - GL_FRAGMENT_DEPTH" << std::endl;
            break;

        case 'C':
        case 'c':
            glFogi(GL_FOG_COORDINATE_SOURCE, GL_FOG_COORDINATE);
            std::cout << "SET : GL_FOG_COORDINATE_SOURCE - GL_FOG_COORDINATE" << std::endl;
            break;

        case '1':
            g_f1 += ITER_DELTA;
            break;

        case '2':
            g_f2 += ITER_DELTA;
            break;

        case '3':
            g_f3 += ITER_DELTA;
            break;

        case '4':
            g_f1 -= ITER_DELTA;
            if(g_f1 < ITER_DELTA)
                g_f1 = ITER_DELTA;
            break;

        case '5':
            g_f2 -= ITER_DELTA;
            if(g_f2 < ITER_DELTA)
                g_f2 = ITER_DELTA;
            break;

        case '6':
            g_f3 -= ITER_DELTA;
            if(g_f3 < ITER_DELTA)
                g_f3 = ITER_DELTA;
            break;

        case 'B':
        case 'b':
            glMatrixMode(GL_MODELVIEW);
            glTranslated(0.0, 0.0, -0.5);
            break;

        case 'F':
        case 'f':
            glMatrixMode(GL_MODELVIEW);
            glTranslated(0.0, 0.0, 0.5);
            break;

        case 27:
            exit(0);
            break;

        default:
            break;
    }

    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    info();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
