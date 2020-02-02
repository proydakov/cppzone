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

#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE

#ifdef _MSC_VER
#   define GL_BGR 0x80E0
#endif // _MSC_VER

constexpr GLdouble WORLD_SIZE_KOEF = 1.0;
constexpr GLdouble ROTATION_DELTA  = 0.5;
constexpr char const* const COMMENT = "Press 'e' for enable or 'd' for disable AntiAliasing. Press ESC for exit.";

class tcapplication : public application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds delta) override;
    void draw() override;

    void info() override;
    void keyboard(SDL_Event const& e) override;

private:
    void initAntiAliasing();

private:

    GLdouble m_rotationAngle = 0;
    bool m_AntiAliasing = true;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    GLfloat values[2];

    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    std::cout << "GL_LINE_WIDTH_GRANULARITY : " << values[0] << std::endl;

    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    std::cout << "GL_LINE_WIDTH_RANGE : " << values[0] << std::endl;

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5);

    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void tcapplication::resize(std::size_t w, std::size_t h)
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

void tcapplication::update(std::chrono::microseconds)
{
    m_rotationAngle += ROTATION_DELTA;
    if(m_rotationAngle > 360.0)
        m_rotationAngle -= 360.0;
}

void tcapplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    initAntiAliasing();

    glColor3d(0.0, 1.0, 0.0);
    glPushMatrix();
    glRotated(-m_rotationAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    {
        glVertex2d(-0.5, 0.5);
        glVertex2d(0.5, -0.5);
    }
    glEnd();
    glPopMatrix();

    glColor3d(0.0, 0.0, 1.0);
    glPushMatrix();
    glRotated(m_rotationAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
    {
        glVertex2d(0.5, 0.5);
        glVertex2d(-0.5, -0.5);
    }
    glEnd();
}

void tcapplication::info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void tcapplication::keyboard(SDL_Event const& e)
{
    switch (e.key.keysym.sym)
    {
        case SDLK_e:
            m_AntiAliasing = true;
            break;

        case SDLK_d:
            m_AntiAliasing = false;
            break;

        default:
            break;
    }
}

void tcapplication::initAntiAliasing()
{
    if(m_AntiAliasing)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    else
        glBlendFunc(GL_ONE, GL_ONE);
}

int main( int argc, char* argv[] )
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
