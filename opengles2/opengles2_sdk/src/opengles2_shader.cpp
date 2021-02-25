#include <iostream>

#include "opengles2_sdk/opengles2_shader.h"

opengles2_shader::opengles2_shader() noexcept
    : m_shader(0)
{
}

opengles2_shader::~opengles2_shader()
{
    unload();
}

bool opengles2_shader::load(GLenum type, const char * const fpath, const char * const shader_text) noexcept
{
    unload();

    // Create the shader object
    GLuint shader = glCreateShader(type);

    if (shader == 0) {
        return false;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shader_text, nullptr);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[static_cast<size_t>(infoLen)]();
            glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
            std::cerr << "Error compiling shader(" << fpath << "):\n" << infoLog << std::endl;
            delete [] infoLog;
        }
        glDeleteShader(shader);
        return false;
    }

    m_shader = shader;

    return true;
}

void opengles2_shader::unload() noexcept
{
    if (m_shader) {
        glDeleteShader(m_shader);
        m_shader = 0;
    }
}
