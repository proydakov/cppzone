/*
 *  Copyright (c) 2021 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include "resource.h"

#include <opengles2_sdk/opengles2_program.h>
#include <opengles2_sdk/opengles2_application.h>

class tcapplication : public opengles2_application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds) override;
    void draw() override;

private:
   opengles2_program m_program;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h)
    : opengles2_application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    auto vShaderStr = opengles2_application::load_resource(RESOURCE_DIRECTORY, "vshader.glsl");
    auto fShaderStr = opengles2_application::load_resource(RESOURCE_DIRECTORY, "fshader.glsl");

    if (!m_program.load(vShaderStr.c_str(), fShaderStr.c_str(), {}, {}))
    {
        panic();
    }

    glClearColor( 0.0f, 0.0f, 1.0f, 0.0f );
}

void tcapplication::resize(std::size_t w, std::size_t h)
{
    // Set the viewport
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void tcapplication::update(std::chrono::microseconds)
{
}

void tcapplication::draw()
{
    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
                            -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f };

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(m_program.get_id());

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray( 0 );

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main( int argc, char* argv[] )
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
