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

struct tprogram_object
{
    bool load(std::string prefix)
    {
        auto [vShaderPath, vShaderStr] = opengles2_application::load_text_resource(SHADERS_DIRECTORY, prefix + "_vshader.glsl");
        auto [fShaderPath, fShaderStr] = opengles2_application::load_text_resource(SHADERS_DIRECTORY, prefix + "_fshader.glsl");

        if (!m_vertex_shader.load(GL_VERTEX_SHADER, vShaderPath.c_str(), vShaderStr.c_str()) ||
            !m_fragment_shader.load(GL_FRAGMENT_SHADER, fShaderPath.c_str(), fShaderStr.c_str()))
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

        auto const vpLoc = m_program.get_uniform_location("vVPMatrix");
        if (!vpLoc)
        {
            return false;
        }

        m_vpLoc = *vpLoc;

        auto const modelLoc = m_program.get_uniform_location("vModelMatrix");
        if (!modelLoc)
        {
            return false;
        }

        m_modelLoc = *modelLoc;

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
    GLint m_vpLoc{};
    GLint m_modelLoc{};
    GLint m_samplerLoc{};
};

struct tprogram_outline
{
    bool load(std::string prefix)
    {
        auto [vShaderPath, vShaderStr] = opengles2_application::load_text_resource(SHADERS_DIRECTORY, prefix + "_vshader.glsl");
        auto [fShaderPath, fShaderStr] = opengles2_application::load_text_resource(SHADERS_DIRECTORY, prefix + "_fshader.glsl");

        if (!m_vertex_shader.load(GL_VERTEX_SHADER, vShaderPath.c_str(), vShaderStr.c_str()) ||
            !m_fragment_shader.load(GL_FRAGMENT_SHADER, fShaderPath.c_str(), fShaderStr.c_str()))
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

        auto const normalLoc = m_program.get_attribute_location("vNormal");
        if (!normalLoc)
        {
            return false;
        }

        m_normalLoc = *normalLoc;

        auto const vpLoc = m_program.get_uniform_location("vVPMatrix");
        if (!vpLoc)
        {
            return false;
        }

        m_vpLoc = *vpLoc;

        auto const modelLoc = m_program.get_uniform_location("vModelMatrix");
        if (!modelLoc)
        {
            return false;
        }

        m_modelLoc = *modelLoc;

        return true;
    }

    opengles2_shader m_vertex_shader;
    opengles2_shader m_fragment_shader;
    opengles2_program m_program;

    GLuint m_positionLoc{};
    GLuint m_normalLoc{};
    GLint m_vpLoc{};
    GLint m_modelLoc{};
};

class tcapplication : public opengles2_application
{
public:
    tcapplication(int, char*[], std::size_t, std::size_t);

    void init() override;
    void resize(std::size_t, std::size_t) override;
    void update(std::chrono::microseconds) override;
    void on_event(SDL_Event const&) override;
    void draw() override;

private:
    float m_angle{};
    opengles2_matrix m_vp_matrix;

    tprogram_object m_object_program;

    opengles2_matrix m_floor_model;
    opengles2_texture m_floor_texture;

    GLfloat const m_floor_vertices[12] = {
        -1.5f, -0.5f,  1.5f, // Position 0
        -1.5f, -0.5f, -1.5f, // Position 1
         1.5f, -0.5f, -1.5f, // Position 2
         1.5f, -0.5f,  1.5f, // Position 3
    };

    GLfloat const m_floor_textures[8] = {
         0.0f,  0.0f,        // TexCoord 0
         0.0f,  1.0f,        // TexCoord 1
         1.0f,  1.0f,        // TexCoord 2
         1.0f,  0.0f         // TexCoord 3
    };

    GLushort const m_floor_indices[6] = {
        0, 1, 2, 0, 2, 3
    };

    opengles2_matrix m_cube_model[2];
    opengles2_texture m_cube_texture;

    tprogram_outline m_outline_program;

    bool m_show{true};
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h)
    : opengles2_application(argc, argv, w, h)
{
    opengles2_matrix::load_identity(m_floor_model);

    opengles2_matrix::load_identity(m_cube_model[0]);
    opengles2_matrix::translate(m_cube_model[0], 0.6f, 0.0, 0.2f);

    opengles2_matrix::load_identity(m_cube_model[1]);
    opengles2_matrix::translate(m_cube_model[1], -0.6f, 0.0, -0.2f);
}

void tcapplication::init()
{
    if (!m_object_program.load("object"))
    {
        panic();
    }

    if (!m_outline_program.load("outline"))
    {
        panic();
    }

    if (!opengles2_application::load_tga(m_cube_texture, TEXTURES_DIRECTORY, "cube.tga"))
    {
        panic();
    }

    if (!opengles2_application::load_tga(m_floor_texture, TEXTURES_DIRECTORY, "floor.tga"))
    {
        panic();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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

    m_angle += (count * 15.0f);
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
    opengles2_matrix::translate(modelview, 0.0, 0.5, -3.0);
    opengles2_matrix::rotate(modelview, -32.5, 1.0, 0.0, 0.0);

    // Rotate the scene
    opengles2_matrix::rotate(modelview, m_angle, 0.0, 1.0, 0.0);

    // Compute the final MVP by multiplying the 
    // modevleiw and perspective matrices together
    opengles2_matrix::multiply(m_vp_matrix, modelview, perspective);
}

void tcapplication::draw()
{
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // floor

    glStencilMask(0x00);

    {
        auto const positionLoc = m_object_program.m_positionLoc;
        auto const textureLoc = m_object_program.m_textureLoc;
        auto const vpLoc = m_object_program.m_vpLoc;
        auto const modelLoc = m_object_program.m_modelLoc;
        auto const samplerLoc = m_object_program.m_samplerLoc;

        glUseProgram(m_object_program.m_program.get_id());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_floor_texture.get_id());

        glUniform1i(samplerLoc, 0);
        glUniformMatrix4fv(vpLoc, 1, GL_FALSE, m_vp_matrix.toUniform());
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_floor_model.toUniform());

        glEnableVertexAttribArray(positionLoc);
        glEnableVertexAttribArray(textureLoc);

        glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_floor_vertices);
        glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), m_floor_textures);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(std::size(m_floor_indices)), GL_UNSIGNED_SHORT, m_floor_indices);

        glDisableVertexAttribArray(positionLoc);
        glDisableVertexAttribArray(textureLoc);
    }

    // object

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    {
        auto const positionLoc = m_object_program.m_positionLoc;
        auto const textureLoc = m_object_program.m_textureLoc;
        auto const vpLoc = m_object_program.m_vpLoc;
        auto const modelLoc = m_object_program.m_modelLoc;
        auto const samplerLoc = m_object_program.m_samplerLoc;

        glUseProgram(m_object_program.m_program.get_id());

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_cube_texture.get_id());

        glUniform1i(samplerLoc, 1);
        glUniformMatrix4fv(vpLoc, 1, GL_FALSE, m_vp_matrix.toUniform());

        glEnableVertexAttribArray(positionLoc);
        glEnableVertexAttribArray(textureLoc);

        for(int i = 0; i < 2; i++)
        {
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_cube_model[i].toUniform());
            glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), opengles2_mesh::cubeVertices());
            glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), opengles2_mesh::cubeTextures());

            glDrawElements(GL_TRIANGLES, opengles2_mesh::cubeIndicesSize(), GL_UNSIGNED_SHORT, opengles2_mesh::cubeIndices());
        }

        glDisableVertexAttribArray(positionLoc);
        glDisableVertexAttribArray(textureLoc);
    }

    // outline

    if (m_show)
    {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        {
            auto const positionLoc = m_outline_program.m_positionLoc;
            auto const normalLoc = m_outline_program.m_normalLoc;
            auto const vpLoc = m_outline_program.m_vpLoc;
            auto const modelLoc = m_outline_program.m_modelLoc;

            glUseProgram(m_outline_program.m_program.get_id());

            glUniformMatrix4fv(vpLoc, 1, GL_FALSE, m_vp_matrix.toUniform());

            glEnableVertexAttribArray(positionLoc);
            glEnableVertexAttribArray(normalLoc);

            for(int i = 0; i < 2; i++)
            {
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_cube_model[i].toUniform());
                glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), opengles2_mesh::cubeVertices());
                glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), opengles2_mesh::cubeVertices());

                glDrawElements(GL_TRIANGLES, opengles2_mesh::cubeIndicesSize(), GL_UNSIGNED_SHORT, opengles2_mesh::cubeIndices());
            }

            glDisableVertexAttribArray(positionLoc);
            glDisableVertexAttribArray(normalLoc);
        }

        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
    }
}

void tcapplication::on_event(SDL_Event const& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case 'S':
            case 's':
                m_show = true;
                break;

            case 'H':
            case 'h':
                m_show = false;
                break;

            default:
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    tcapplication app(argc, argv, 640, 480);
    return app.run();
}
