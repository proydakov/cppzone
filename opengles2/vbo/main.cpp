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
#include <opengles2_sdk/opengles2_mesh.h>
#include <opengles2_sdk/opengles2_shader.h>
#include <opengles2_sdk/opengles2_texture.h>
#include <opengles2_sdk/opengles2_program.h>
#include <opengles2_sdk/opengles2_application.h>

struct tprogram
{
    bool load(std::string prefix)
    {
        if (!opengles2_application::load_vertex_shader(m_vertex_shader, SHADERS_DIRECTORY, prefix + "_vshader.glsl") ||
            !opengles2_application::load_fragment_shader(m_fragment_shader, SHADERS_DIRECTORY, prefix + "_fshader.glsl"))
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
        if (!textureLoc)
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
    ~tcapplication() override;

    void init() override;
    void resize(std::size_t, std::size_t) override;
    void update(std::chrono::microseconds) override;
    void draw() override;
    void on_event(SDL_Event const&) override;
    void info(std::ostream&) override;

private:
    void draw_general();
    void draw_vbo();
    void info_vbo();

private:
    bool m_vbo_mode{true};
    float m_angle{};
    opengles2_matrix m_mvpMatrix;

    tprogram m_cube_program;
    opengles2_texture m_cube_texture;

    GLuint m_vertex_array_buffer;
    GLuint m_vertex_index_buffer;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h)
    : opengles2_application(argc, argv, w, h)
{
}

tcapplication::~tcapplication()
{
    glDeleteBuffers(1, &m_vertex_index_buffer);
    glDeleteBuffers(1, &m_vertex_array_buffer);
}

void tcapplication::init()
{
    if (!m_cube_program.load("cube"))
    {
        panic();
    }

    if (!opengles2_application::load_tga(m_cube_texture, TEXTURES_DIRECTORY, "plane.tga"))
    {
        panic();
    }

    glClearColor(0.0f, 0.24f, 0.70f, 0.0f);

    {
        glGenBuffers(1, &m_vertex_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(opengles2_mesh::cubeIndices()), opengles2_mesh::cubeIndices(), GL_STATIC_DRAW);

        GLint size = 0;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        if(sizeof(opengles2_mesh::cubeIndices()) != size)
        {
            glDeleteBuffers(1, &m_vertex_index_buffer);
            panic();
        }
    }

    {
        // build VAB for GPU
        GLfloat buffer[48 + 72] = {};

        for(size_t i = 0; i < 24; i++)
        {
            size_t voffset = i * 3;
            size_t toffset = i * 2;
            size_t boffset = i * 5;

            buffer[boffset + 0] = opengles2_mesh::cubeVertices()[voffset + 0];
            buffer[boffset + 1] = opengles2_mesh::cubeVertices()[voffset + 1];
            buffer[boffset + 2] = opengles2_mesh::cubeVertices()[voffset + 2];

            buffer[boffset + 3] = opengles2_mesh::cubeTextures()[toffset + 0];
            buffer[boffset + 4] = opengles2_mesh::cubeTextures()[toffset + 1];
        }

        glGenBuffers(1, &m_vertex_array_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

        GLint size = 0;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        if(sizeof(buffer) != size)
        {
            glDeleteBuffers(1, &m_vertex_array_buffer);
            panic();
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    info_vbo();
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
    opengles2_matrix::translate(modelview, 0.0, 0.0, -2.0);
    opengles2_matrix::rotate(modelview, -22.5, 1.0, 0.0, 0.0);

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

    if (m_vbo_mode)
    {
        draw_vbo();
    }
    else
    {
        draw_general();
    }

    glDisable(GL_DEPTH_TEST);
}

void tcapplication::draw_vbo()
{
    auto const positionLoc = m_cube_program.m_positionLoc;
    auto const textureLoc = m_cube_program.m_textureLoc;
    auto const mvpLoc = m_cube_program.m_mvpLoc;
    auto const samplerLoc = m_cube_program.m_samplerLoc;

    // Use the program object
    glUseProgram(m_cube_program.m_program.get_id());

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cube_texture.get_id());

    // Set the sampler texture unit to 0
    glUniform1i(samplerLoc, 0);

    // Set MVP matrix
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, m_mvpMatrix.toUniform());

    // vertex_buffer is retrieved from glGenBuffers
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_array_buffer);

    // index_buffer is retrieved from glGenBuffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertex_index_buffer);

    // Attribute indexes were received from calls to glGetAttribLocation, or passed into glBindAttribLocation.
    glEnableVertexAttribArray(positionLoc);
    glEnableVertexAttribArray(textureLoc);

    // vertex_stride is the size of bytes of each vertex in the buffer object
    // vertex_position_offset and kin are the offset in bytes of the position data
    // in each vertex. For example if your vertex structure is
    // [ position, texcoord, normal ] then position vertex_position_offset will
    // have offset 0, vertex_texcoord_offset is 12 (position is 3 * sizeof(float)
    // bytes large, and texcoord comes just after) and vertex_normal_offset is
    // 20 = 5 * sizeof(float).
    GLintptr vertex_position_offset = 0 * sizeof(float);
    GLintptr vertex_texcoord_offset = 3 * sizeof(float);

    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, false, sizeof(GLfloat) * 5, (GLvoid*) vertex_position_offset);
    glVertexAttribPointer(textureLoc, 2, GL_FLOAT, false, sizeof(GLfloat) * 5, (GLvoid*) vertex_texcoord_offset);

    // num_vertices is the number of verts in your vertex_data.
    // index_data is an array of unsigned int offsets into vertex_data.
    glDrawElements(GL_TRIANGLES, opengles2_mesh::cubeIndicesSize(), GL_UNSIGNED_SHORT, nullptr);

    glDisableVertexAttribArray(positionLoc);
    glDisableVertexAttribArray(textureLoc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void tcapplication::draw_general()
{
    auto const positionLoc = m_cube_program.m_positionLoc;
    auto const textureLoc = m_cube_program.m_textureLoc;
    auto const mvpLoc = m_cube_program.m_mvpLoc;
    auto const samplerLoc = m_cube_program.m_samplerLoc;

    // Use the program object
    glUseProgram(m_cube_program.m_program.get_id());

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_cube_texture.get_id());

    // Set the sampler texture unit to 0
    glUniform1i(samplerLoc, 0);

    // Set MVP matrix
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, m_mvpMatrix.toUniform());

    glEnableVertexAttribArray(positionLoc);
    glEnableVertexAttribArray(textureLoc);

    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), opengles2_mesh::cubeVertices());
    glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), opengles2_mesh::cubeTextures());

    glDrawElements(GL_TRIANGLES, opengles2_mesh::cubeIndicesSize(), GL_UNSIGNED_SHORT, opengles2_mesh::cubeIndices());

    glDisableVertexAttribArray(positionLoc);
    glDisableVertexAttribArray(textureLoc);
}

void tcapplication::on_event(SDL_Event const& e)
{
    auto const mode = m_vbo_mode;

    switch (e.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case 'V':
            case 'v':
                m_vbo_mode = true;
                break;

            case 'S':
            case 's':
                m_vbo_mode = false;
                break;

            default:
                break;
        }
    }

    if (mode != m_vbo_mode)
    {
        info_vbo();
    }
}

void tcapplication::info(std::ostream& os)
{
    os << "Change CullFace mode (default: GL_FRONT):\n";
    os << "Press 'V' for enable VBO rendering\n";
    os << "Press 'S' for disable VBO rendering\n";
}

void tcapplication::info_vbo()
{
    std::cout << "Set VBO mode: " << m_vbo_mode << std::endl;
}

int main(int argc, char* argv[])
{
    tcapplication app(argc, argv, 640, 640);
    return app.run();
}
