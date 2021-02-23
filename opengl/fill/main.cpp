/*
 *  Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
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
#include <string>
#include <iostream>
#include <algorithm>

#include <opengl_sdk/application.h>

class tcapplication : public application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds) override;
    void draw() override;

    void info(std::ostream&) override;
    void on_event(SDL_Event const& e) override;

private:
    using point = std::pair<GLdouble, GLdouble>;

    void nextState();
    void drawAllHexagons();
    void drawAllSquares();
    void drawAllTriangles();
    void drawHexagon(point center, GLdouble width, GLdouble height);
    void drawSquare(point center, GLdouble size);
    void drawTriangle(point p1, point p2, point p3);
    void clearScreen();
    void setPenColor(GLfloat red, GLfloat green, GLfloat blue);

private:
    static constexpr const int STATE_MIN  = 1;
    static constexpr const int STATE_MAX  = 3;

    static constexpr int STATE_DRAW_TRIANGLES = STATE_MIN;
    static constexpr int STATE_DRAW_SQUARES   = STATE_MIN + 1;
    static constexpr int STATE_DRAW_HEXAGONS  = STATE_MAX;

    static constexpr int MIN_WIDTH = 100;
    static constexpr int MIN_HEIGHT = 100;

    static constexpr int OBJECT_COUNT = 9;

    int g_drawTypeState = STATE_MIN;
    std::size_t g_width  = 400;
    std::size_t g_height = 400;
    bool g_clear = false;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void tcapplication::resize(std::size_t w, std::size_t h)
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

void tcapplication::update(std::chrono::microseconds)
{
}

void tcapplication::setPenColor(GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3d(red, green, blue);
}

void tcapplication::clearScreen()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void tcapplication::drawTriangle(point p1, point p2, point p3)
{
    glBegin(GL_POLYGON);
    {
        glVertex2d(p1.first, p1.second);
        glVertex2d(p2.first, p2.second);
        glVertex2d(p3.first, p3.second);
    }
    glEnd();
}

void tcapplication::drawSquare(point center, GLdouble size)
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

void tcapplication::drawHexagon(point center, GLdouble width, GLdouble height)
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

void tcapplication::drawAllTriangles()
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    setPenColor(0, 0, 1);

    auto size = GLdouble(std::min(g_width, g_height) / OBJECT_COUNT);
    auto height = GLdouble(std::sqrt(size * size - size / 2 * size / 2));

    auto widthMax = GLdouble(g_width) + size;
    auto heightMax = GLdouble(g_height) + height;

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

void tcapplication::drawAllSquares()
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    setPenColor(0, 0, 1);

    auto const min_val = std::min(g_width, g_height);
    GLdouble size = GLdouble(min_val / OBJECT_COUNT);
    GLdouble doubleSize = size * 2;

    GLdouble widthMax = GLdouble(g_width) + size;
    GLdouble heightMax = GLdouble(g_height) + size;

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

void tcapplication::drawAllHexagons()
{
    glClearColor(1.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLdouble size = GLdouble(std::min(g_width, g_height) / OBJECT_COUNT);
    GLdouble stepSize = size * 1.5;
    GLdouble height = std::sqrt(size * size - size / 2 * size / 2);

    GLdouble widthMax = GLdouble(g_width) + size;
    GLdouble heightMax = GLdouble(g_height) + height;

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

void tcapplication::nextState()
{
    g_drawTypeState++;
    if(g_drawTypeState > STATE_MAX)
        g_drawTypeState = STATE_MIN;
}

void tcapplication::draw()
{
    if(g_clear)
    {
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
            abort();
            break;
    }
}

void tcapplication::info(std::ostream& os)
{
    os << "Press 'n' for next symbol.\n";
}

void tcapplication::on_event(SDL_Event const& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
        {
            case SDLK_n:
                nextState();
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
