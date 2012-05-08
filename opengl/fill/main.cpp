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

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>

#include <GL/glut.h>

typedef std::pair<GLdouble, GLdouble> point;

const int STATE_MIN  = 1;
const int STATE_MAX  = 3;

const int STATE_DRAW_TRIANGLES = STATE_MIN;
const int STATE_DRAW_SQUARES   = STATE_MIN + 1;
const int STATE_DRAW_HEXAGONS  = STATE_MAX;

const int MIN_WIDTH = 100;
const int MIN_HEIGHT = 100;

const int OBJECT_COUNT = 9;


static int g_drawTypeState = STATE_MIN;
static int g_width  = 400;
static int g_height = 400;
static bool g_clear = false;


void setPenColor(GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3d(red, green, blue);
}

void clearScreen()
{
    std::cout << "clearScreen" << std::endl;

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawTriangle(point p1, point p2, point p3)
{
    glBegin(GL_POLYGON);
    {
        glVertex2d(p1.first, p1.second);
        glVertex2d(p2.first, p2.second);
        glVertex2d(p3.first, p3.second);
    }
    glEnd();
}

void drawSquare(point center, GLdouble size)
{
    glBegin(GL_POLYGON);
    {
        glVertex2d(center.first - size / 2, center.second + size / 2);
        glVertex2d(center.first + size / 2, center.second + size / 2);
        glVertex2d(center.first + size / 2, center.second - size / 2);
        glVertex2d(center.first - size / 2, center.second - size / 2);
    }
    glEnd();
}

void drawHexagon(point center, GLdouble width, GLdouble height)
{
    glBegin(GL_POLYGON);
    {
        glVertex2d(center.first - width / 4, center.second + height / 2);
        glVertex2d(center.first + width / 4, center.second + height / 2);
        glVertex2d(center.first + width / 2, center.second);
        glVertex2d(center.first + width / 4, center.second - height / 2);
        glVertex2d(center.first - width / 4, center.second - height / 2);
        glVertex2d(center.first - width / 2, center.second);
    }
    glEnd();
}

void drawAllTriangles()
{
    std::cout << "drawAllTriangles" << std::endl;

    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    setPenColor(0, 0, 1);

    GLdouble size = std::min(g_width, g_height) / OBJECT_COUNT;
    GLdouble height = sqrt(size * size - size / 2 * size / 2);

    GLdouble widthMax = g_width + size;
    GLdouble heightMax = g_height + height;

    int rowCounter = 0;

    for(GLdouble heightUpPosition = 0; heightUpPosition < heightMax; heightUpPosition += height) {
        GLdouble heightDownPosition = heightUpPosition + height;

        GLdouble delta = rowCounter % 2 ? size / 2 : 0;

        for(GLdouble upPosition = delta; upPosition < widthMax; upPosition += size) {

            GLdouble downLeftPosition = upPosition + -size / 2;
            GLdouble downRightPosition = upPosition + size / 2;

            drawTriangle(point(upPosition, heightUpPosition), 
                         point(downLeftPosition, heightDownPosition),
                         point(downRightPosition, heightDownPosition));
        }
        rowCounter++;
    }
}

void drawAllSquares()
{
    std::cout << "drawAllSquares" << std::endl;

    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    setPenColor(0, 0, 1);

    GLdouble size = std::min(g_width, g_height) / OBJECT_COUNT;
    GLdouble doubleSize = size * 2;

    GLdouble widthMax = g_width + size;
    GLdouble heightMax = g_height + size;

    GLdouble delta = 0;

    for(GLdouble i = size / 2; i < widthMax; i += doubleSize) {
        for(GLdouble j = size / 2; j < heightMax; j += size) {
            delta = 0;
            if(static_cast<int>((i + j) / size) % 2)
                delta = size;
            drawSquare(point(i + delta, j), size);
        }
    }
}

void drawAllHexagons()
{
    std::cout << "drawAllHexagons" << std::endl;

    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLdouble size = std::min(g_width, g_height) / OBJECT_COUNT;
    GLdouble stepSize = size * 1.5;
    GLdouble height = sqrt(size * size - size / 2 * size / 2);

    GLdouble widthMax = g_width + size;
    GLdouble heightMax = g_height + height;

    int rowCounter = 0;

    for(GLdouble horizontalPosition = 0; horizontalPosition < heightMax; horizontalPosition += height) {
        
        int rowCounterOst = rowCounter % 3;
        if(rowCounterOst == 0) {
            setPenColor(1, 0, 0);
        }
        else if(rowCounterOst == 1) {
            setPenColor(0, 1, 0);
        }
        else {
            setPenColor(0, 0, 1);
        }

        for(GLdouble verticalPosition = 0; verticalPosition < widthMax; verticalPosition += stepSize) {
            drawHexagon(point(verticalPosition, horizontalPosition), size, height);
        }
        rowCounter++;
    }

    rowCounter = 0;

    for(GLdouble horizontalPosition = height / 2; horizontalPosition < heightMax; horizontalPosition += height) {
        
        int rowCounterOst = rowCounter % 3;
        if(rowCounterOst == 0) {
            setPenColor(0, 0, 1);
        }
        else if(rowCounterOst == 1) {
            setPenColor(1, 0, 0);
        }
        else {
            setPenColor(0, 1, 0);
        }

        for(GLdouble verticalPosition = stepSize / 2; verticalPosition < widthMax; verticalPosition += stepSize) {
            drawHexagon(point(verticalPosition, horizontalPosition), size, height);
        }
        rowCounter++;
    }
}

void nextState()
{
    g_drawTypeState++;
    if(g_drawTypeState > STATE_MAX)
        g_drawTypeState = STATE_MIN;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void display()
{
    if(g_clear) {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        g_clear = false;
        return;
    }
 
    clearScreen();

    switch(g_drawTypeState) {
        case STATE_DRAW_TRIANGLES:
            drawAllTriangles();
            break;

        case STATE_DRAW_SQUARES:
            drawAllSquares();
            break;

        case STATE_DRAW_HEXAGONS:
            drawAllHexagons();
            break;

        default:
            assert(!"INVALID INPUT DATA");
            break;
    }
    glFlush();
}

void reshape(int w, int h)
{
    if(w < MIN_WIDTH)
        w = MIN_WIDTH;

    if(h < MIN_HEIGHT)
        h = MIN_HEIGHT;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble) w, (GLdouble) h, 0.0);

    g_width = w;
    g_height = h;
}

void mouse(int button, int state, int x, int y)
{
    (void)x;
    (void)y;

    if(state != GLUT_DOWN)
        return;

    if(button == GLUT_LEFT_BUTTON)
        nextState();
    
    if(button == GLUT_RIGHT_BUTTON) {
        g_clear = true;
    }

#ifdef __GNUC__
    display();
#endif // __GNUC__
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(g_width, g_height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
