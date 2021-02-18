#pragma once

#include "opengles2.h"

class opengles2_texture final
{
public:
    opengles2_texture();
    ~opengles2_texture();

    bool load(int width, int height, GLint internalformat, GLenum format, GLenum type, GLvoid* pixels);
    void unload();

    GLuint get_id() const;

private:
    GLuint m_texture_id;
};
