#pragma once

#include "opengles2.h"

// https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml

struct opengles2_texture_filter
{
    GLint min_filter{GL_LINEAR};
    GLint mag_filter{GL_LINEAR};
    GLint wrap_s{GL_CLAMP_TO_EDGE};
    GLint wrap_t{GL_CLAMP_TO_EDGE};
};

class opengles2_texture final
{
public:
    opengles2_texture();
    ~opengles2_texture();

    bool load(int w, int h, GLint internalformat, GLenum format, GLenum pixelType, const GLvoid* pixels);
    bool load(int w, int h, GLint internalformat, GLenum format, GLenum pixelType, const GLvoid* pixels, const opengles2_texture_filter&);
    void unload();

    GLuint get_id() const;

private:
    GLuint m_texture_id;
};
