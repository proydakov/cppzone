#pragma once

#include "opengles2.h"

class opengles2_shader final
{
public:
    opengles2_shader() noexcept;
    ~opengles2_shader();

    bool load(GLenum type, const char* const fpath, const char* const shader_text) noexcept;
    void unload() noexcept;

    GLuint get_id() const noexcept
    {
        return m_shader;
    }

private:
    GLuint m_shader;
};
