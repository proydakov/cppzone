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

#include <opengles2_sdk/opengles2_math.h>
#include <opengles2_sdk/opengles2_shader.h>
#include <opengles2_sdk/opengles2_texture.h>
#include <opengles2_sdk/opengles2_program.h>
#include <opengles2_sdk/opengles2_application.h>

struct tprogram
{
    bool load(std::string prefix)
    {
        auto vShaderStr = opengles2_application::load_resource(SHADERS_DIRECTORY, prefix + "_vshader.glsl");
        auto fShaderStr = opengles2_application::load_resource(SHADERS_DIRECTORY, prefix + "_fshader.glsl");

        if (!m_vertex_shader.load(GL_VERTEX_SHADER, vShaderStr.c_str()) ||
            !m_fragment_shader.load(GL_FRAGMENT_SHADER, fShaderStr.c_str()))
        {
            return false;
        }

        if (!m_program.load(m_vertex_shader, m_fragment_shader))
        {
            return false;
        }

        auto const positionLoc = m_program.get_attribute_location("vPosition");
        if (!positionLoc)
        {
            return false;
        }

        m_positionLoc = *positionLoc;

        auto const textureLoc = m_program.get_attribute_location("vTexCoord");
        if (!positionLoc)
        {
            return false;
        }

        m_textureLoc = *textureLoc;

        auto const mvpLoc = m_program.get_uniform_location("vMvpMatrix");
        if (!mvpLoc)
        {
            return false;
        }

        m_mvpLoc = *mvpLoc;

        auto const samplerLoc = m_program.get_uniform_location("fTexture");
        if (!samplerLoc)
        {
            return false;
        }

        m_samplerLoc = *samplerLoc;

        return true;
    }

    opengles2_shader m_vertex_shader;
    opengles2_shader m_fragment_shader;
    opengles2_program m_program;

    GLuint m_positionLoc{};
    GLuint m_textureLoc{};
    GLint m_mvpLoc{};
    GLint m_samplerLoc{};
};

class tcapplication : public opengles2_application
{
public:
    tcapplication(int, char*[], std::size_t, std::size_t);

    void init() override;
    void resize(std::size_t, std::size_t) override;
    void update(std::chrono::microseconds) override;
    void draw() override;
    void on_event(SDL_Event const&) override;
    void info(std::ostream&) override;

private:
    GLenum m_mode{GL_BACK};
    float m_angle{};
    opengles2_matrix m_mvpMatrix;

    tprogram m_cube_program;
    opengles2_texture m_cube_texture;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h)
    : opengles2_application(argc, argv, w, h)
{
}

void tcapplication::init()
{
    if (!m_cube_program.load("cube"))
    {
        panic();
    }

    if (!opengles2_application::load_tga(m_cube_texture, TEXTURES_DIRECTORY, "cube.tga"))
    {
        panic();
    }

    glClearColor(0.0f, 0.24f, 0.70f, 0.0f);
}

void tcapplication::resize(std::size_t w, std::size_t h)
{
    // Set the viewport
    glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void tcapplication::update(std::chrono::microseconds delta)
{
    // Compute a rotation angle based on time to rotate the cube
    auto const count = static_cast<float>(delta.count()) / 1'000'000;

    m_angle += (count * 25.0f);
    if( m_angle >= 360.0f ) {
        m_angle -= 360.0f;
    }

   // Compute the window aspect ratio
   float aspect = static_cast<float>(getWidth()) / static_cast<float>(getHeight());

    // Generate a perspective matrix with a 60 degree FOV
    opengles2_matrix perspective;
    opengles2_matrix::load_identity(perspective);
    opengles2_matrix::perspective(perspective, 60.0f, aspect, 1.0f, 20.0f);

    // Generate a model view matrix to rotate/translate the cube
    opengles2_matrix modelview;
    opengles2_matrix::load_identity(modelview);

    // Translate away from the viewer
    opengles2_matrix::translate(modelview, 0.0, 0.0, -3.0);
    opengles2_matrix::rotate(modelview, -35.0, 1.0, 0.0, 0.0);

    // Rotate the cube
    opengles2_matrix::rotate(modelview, m_angle, 0.0, 1.0, 0.0);

    // Compute the final MVP by multiplying the 
    // modevleiw and perspective matrices together
    opengles2_matrix::multiply(m_mvpMatrix, modelview, perspective);
}

void tcapplication::draw()
{
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(m_mode);

    {
        auto const positionLoc = m_cube_program.m_positionLoc;
        auto const textureLoc = m_cube_program.m_textureLoc;
        auto const mvpLoc = m_cube_program.m_mvpLoc;
        auto const samplerLoc = m_cube_program.m_samplerLoc;

        GLfloat const vertices[] =
        {
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
        };

       GLfloat const textures[] =
       {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
        };

        GLushort const indices[] =
        {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 15, 14,
            12, 14, 13,
            16, 17, 18,
            16, 18, 19,
            20, 23, 22,
            20, 22, 21,
        };

        // Use the program object
        glUseProgram(m_cube_program.m_program.get_id());

        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_cube_texture.get_id());

        // Set the sampler texture unit to 0
        glUniform1i(samplerLoc, 0);

        glEnableVertexAttribArray(positionLoc);
        glEnableVertexAttribArray(textureLoc);

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, m_mvpMatrix.toUniform());

        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertices);
        glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), textures);

        glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_SHORT, indices);

        glDisableVertexAttribArray(positionLoc);
        glDisableVertexAttribArray(textureLoc);
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void tcapplication::on_event(SDL_Event const& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case 'F':
            case 'f':
                m_mode = GL_FRONT;
                break;

            case 'B':
            case 'b':
                m_mode = GL_BACK;
                break;

            default:
                break;
        }
    }
}

void tcapplication::info(std::ostream& os)
{
    os << "Change CullFace mode (default: GL_FRONT):\n";
    os << "Press 'F' for GL_FRONT\n";
    os << "Press 'B' for GL_BACK\n";
}

int main(int argc, char* argv[])
{
    tcapplication app(argc, argv, 640, 640);

    return app.run();
}
