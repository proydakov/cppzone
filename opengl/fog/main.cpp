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

const GLdouble WORLD_SIZE_KOEF = 2.5;
const int SPHERE_PRECESSION = 20;

const std::string COMMENT = "Press 'f' for select fog mode. Press ESC for exit.";
GLint g_fogMode = 0;

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void init()
{
    GLfloat position[] = { 0.5, 0.5, 3.0, 0.0 };

    glEnable(GL_DEPTH_TEST);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    {
        GLfloat ambient_mat[3]  = { (GLfloat) 0.1745, (GLfloat) 0.01175, (GLfloat) 0.01175 };
        GLfloat diffuse_mat[3]  = { (GLfloat) 0.61424, (GLfloat) 0.04136, (GLfloat) 0.04136 };
        GLfloat specular_mat[3] = { (GLfloat) 0.727811, (GLfloat) 0.626959, (GLfloat) 0.626959 };
        GLfloat shininess = (GLfloat) (0.6 * 128.0);

        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_mat);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular_mat);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }

    GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
    glEnable(GL_FOG);
    {
        g_fogMode = GL_EXP;
        glFogi(GL_FOG_MODE, g_fogMode);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, (GLfloat) 0.35);
        glHint(GL_FOG_HINT, GL_DONT_CARE);
        glFogf(GL_FOG_START, (GLfloat) 1.0);
        glFogf(GL_FOG_END, (GLfloat) 5.0);
    }

    glClearColor(fogColor[0], fogColor[1], fogColor[2], fogColor[3]);
}

void renderSphere(GLdouble x, GLdouble y, GLdouble z, GLdouble redius = 0.4)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glutSolidSphere(redius, SPHERE_PRECESSION, SPHERE_PRECESSION);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    renderSphere(-2.0, -0.5, -1.0);
    renderSphere(-1.0, -0.5, -2.0);
    renderSphere( 0.0, -0.5, -3.0);
    renderSphere( 1.0, -0.5, -4.0);
    renderSphere( 2.0, -0.5, -5.0);
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

    std::string fogModeComment;

    switch(key) {
        case 'F':
        case 'f':
            if(g_fogMode == GL_EXP) {
                g_fogMode = GL_EXP2;
                fogModeComment = "GL_EXP2";
            }
            else if(g_fogMode == GL_EXP2) {
                g_fogMode = GL_LINEAR;
                fogModeComment = "GL_LINEAR";
            }
            else {
                g_fogMode = GL_EXP;
                fogModeComment = "GL_EXP";
            }
            std::cout << "FOG MODE : " << fogModeComment << std::endl;
            glFogi(GL_FOG_MODE, g_fogMode);
            glutPostRedisplay();
            break;

        case 27:
            exit(0);
            break;

        default:
            break;
    }
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