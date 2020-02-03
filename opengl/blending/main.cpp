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
#include <iostream>

#include <application/application.h>

constexpr char const* const COMMENT = "Press 'e' / 'd' to change the order of drawing objects. Press ESC for exit.";
constexpr GLdouble OBJECT_SIDE = 1;

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
    int g_leftFirst = GL_TRUE;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void tcapplication::resize(std::size_t w, std::size_t h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-OBJECT_SIDE / 2, OBJECT_SIDE * 3 / 2, -OBJECT_SIDE / 2, OBJECT_SIDE * 3 / 2);
}

void tcapplication::update(std::chrono::microseconds)
{
}

void drawTriangle(GLdouble side, GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
    glBegin(GL_POLYGON);
    {
        glColor4d(red, green, blue, alpha);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(side, 0.0, 0.0);
        glVertex3d(0.0, side, 0.0);
    }
    glEnd();
}

void drawLeftTriangle()
{
    glPushMatrix();
    glTranslated(-OBJECT_SIDE / 4, 0, 0);
    drawTriangle(OBJECT_SIDE, 1.0, 1.0, 0.0, 0.75);
    glPopMatrix();
}

void drawRightTriangle()
{
    glPushMatrix();
    glTranslated(OBJECT_SIDE / 4, 0, 0);
    drawTriangle(OBJECT_SIDE, 0.0, 1.0, 1.0, 0.75);
    glPopMatrix();
}

void tcapplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if(g_leftFirst)
    {
        drawLeftTriangle();
        drawRightTriangle();
    }
    else
    {
        drawRightTriangle();
        drawLeftTriangle();
    }
}

void tcapplication::info(std::ostream& os)
{
    os << COMMENT << "\n" << std::endl;
}

void tcapplication::on_event(SDL_Event const& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
                case SDLK_e:
                    g_leftFirst = true;
                    break;

                case SDLK_d:
                    g_leftFirst = false;
                    break;

                default:
                    break;
            }
            break;
    }
}

int main( int argc, char* argv[] )
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
