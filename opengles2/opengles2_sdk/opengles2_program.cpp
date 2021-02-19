#include <iostream>

#include "opengles2_sdk/opengles2_shader.h"
#include "opengles2_sdk/opengles2_program.h"

opengles2_program::opengles2_program() noexcept
    : m_program(0)
{
}

opengles2_program::~opengles2_program()
{
    unload();
}

bool opengles2_program::load(const opengles2_shader& vertex_shader, const opengles2_shader& fragment_shader) noexcept
{
    unload();

    // Create the program object
    GLuint const programObject = glCreateProgram();

    if (programObject == 0) {
        return false;
    }

    glAttachShader(programObject, vertex_shader.get_id());
    glAttachShader(programObject, fragment_shader.get_id());

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[static_cast<size_t>(infoLen)]();
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            std::cerr << "Error linking program:\n" << infoLog << std::endl;
            delete [] infoLog;
        }
        glDeleteProgram(programObject);
        return false;
    }

    // Store the program object
    m_program = programObject;

    return true;
}

std::optional<GLuint> opengles2_program::get_attribute_location(const char* const attribute) const noexcept
{
    auto const val = glGetAttribLocation(m_program, attribute);
    if (val != -1)
    {
        return val;
    }
    else
    {
        return std::nullopt;
    }
}

std::optional<GLint> opengles2_program::get_uniform_location(const char* const uniform) const noexcept
{
    auto const val = glGetUniformLocation(m_program, uniform);
    if (val != -1)
    {
        return val;
    }
    else
    {
        return std::nullopt;
    }
}

void opengles2_program::unload() noexcept
{
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}
