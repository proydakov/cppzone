#pragma once

#include <optional>

#include "opengles2.h"

class opengles2_shader;

class opengles2_program final
{
public:
    opengles2_program() noexcept;
    ~opengles2_program();

    bool load(const opengles2_shader& vertex_shader, const opengles2_shader& fragment_shader) noexcept;
    void unload() noexcept;

    std::optional<GLuint> get_attribute_location(const char* const attribute) const noexcept;
    std::optional<GLint> get_uniform_location(const char* const uniform) const noexcept;

    GLuint get_id() const noexcept
    {
        return m_program;
    }

private:
    GLuint load_shader(GLenum type, const char* shaderSrc) noexcept;

private:
    GLuint m_program;
};
