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

#include <string>
#include <fstream>
#include <iostream>

#include <opengl_sdk/application.h>

class tcapplication : public application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds delta) override;
    void draw() override;

    void info(std::ostream&) override;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
}

void tcapplication::resize(std::size_t w, std::size_t h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w <= h)
        gluOrtho2D(0.0, 30.0, 0.0, 30.0 * (GLdouble) h / (GLdouble) w);
    else
        gluOrtho2D(0.0, 30.0 * (GLdouble) w / (GLdouble) h, 0.0, 30.0);
}

void tcapplication::update(std::chrono::microseconds)
{
}

void tcapplication::info(std::ostream& os)
{
    int red_bits, green_bits, blue_bits, alpha_bits, index_bits;

    glGetIntegerv(GL_RED_BITS, &red_bits);
    glGetIntegerv(GL_GREEN_BITS, &green_bits);
    glGetIntegerv(GL_BLUE_BITS, &blue_bits);
    glGetIntegerv(GL_ALPHA_BITS, &alpha_bits);
    glGetIntegerv(GL_INDEX_BITS, &index_bits);

    os << "GL_RED_BITS : " << red_bits << std::endl;
    os << "GL_GREEN_BITS : " << green_bits << std::endl;
    os << "GL_BLUE_BITS : " << blue_bits << std::endl;
    os << "GL_ALPHA_BITS : " << alpha_bits << std::endl;
    os << "GL_INDEX_BITS : " << index_bits << std::endl;
}

void tcapplication::draw()
{
    auto drawTriangle = []()
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
    };

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    drawTriangle();
}

int main( int argc, char* argv[] )
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
