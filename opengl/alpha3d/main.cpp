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

const std::string COMMENT = "Press 'a' or 'r' key to change state.\nPress ESC for exit...";

const int CYCLE_TIME = 30;
const int SPHERE_PRECESSION = 20;

const GLdouble WORLD_SIZE_KOEF = 1.5;

const GLdouble MAX_Z =  8.0;
const GLdouble MIN_Z = -2.0;
const GLdouble INC_Z =  0.01;

GLdouble solidZ       = MAX_Z;
GLdouble transparentZ = MIN_Z;
GLuint sphereList;
GLuint cubeList;

void info()
{
    std::cout << COMMENT << std::endl;
}

void init()
{
    GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 0.15f };
    GLfloat mat_shininess[] = { 100.0f };
    GLfloat position[] = { 0.5f, 0.5f, 1.0f, 0.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);
        glutSolidSphere(0.4, SPHERE_PRECESSION, SPHERE_PRECESSION);
    glEndList();

    cubeList = glGenLists(1);
    glNewList(cubeList, GL_COMPILE);
        glutSolidCube(0.6);
    glEndList();
}

void display()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat mat_solid[] = { 0.75f, 0.75f, 0.0f, 1.0f };
    GLfloat mat_zero[]  = { 0.0f,  0.0f,  0.0f, 1.0f };
    GLfloat mat_transparent[] = { 0.0f, 0.8f, 0.8f, 0.6f };
    GLfloat mat_emission[]    = { 0.0f, 0.3f, 0.3f, 0.6f };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    {
        glTranslated(-0.15, -0.15, solidZ);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
        glCallList(sphereList);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0.15, 0.15, transparentZ);
        glRotated(15.0, 1.0, 1.0, 0.0);
        glRotated(30.0, 0.0, 1.0, 0.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glCallList(cubeList);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
    glPopMatrix();

    glutSwapBuffers();
}

void cycle(int value)
{
    if(solidZ < MIN_Z || transparentZ >= MAX_Z)
        return;
    else {
        solidZ -= INC_Z;
        transparentZ += INC_Z;
    }

    glutPostRedisplay();
    glutTimerFunc(CYCLE_TIME, cycle, value); 
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 'A':
        case 'a':
            solidZ = MAX_Z;
            transparentZ = MIN_Z;
            glutTimerFunc(CYCLE_TIME, cycle, 0);
            break;

        case 'R':
        case 'r':
            solidZ = MAX_Z;
            transparentZ = MIN_Z;
            glutPostRedisplay();
            break;

        case 27:
            exit(0);
            break;

        default:
            break;
    }
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

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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