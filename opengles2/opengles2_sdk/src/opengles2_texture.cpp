#include "opengles2_sdk/opengles2_texture.h"

// https://habr.com/ru/post/315294

opengles2_texture::opengles2_texture() :
    m_texture_id(0)
{
}

opengles2_texture::~opengles2_texture()
{
    unload();
}

bool opengles2_texture::load(int width, int height, GLint internalformat, GLenum format, GLenum pixelType, const GLvoid* pixels)
{
    opengles2_texture_filter default_filter;
    return load(width, height, internalformat, format, pixelType, pixels, default_filter);
}

bool opengles2_texture::load(int width, int height, GLint internalformat, GLenum format, GLenum pixelType, const GLvoid* pixels, const opengles2_texture_filter& filter)
{
    unload();

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object
    glGenTextures(1, &m_texture_id);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, pixelType, pixels);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter.mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, filter.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, filter.wrap_t);

    return true;
}

void opengles2_texture::unload()
{
    if(m_texture_id) {
        glDeleteTextures(1, &m_texture_id);
        m_texture_id = 0;
    }
}

GLuint opengles2_texture::get_id() const
{
    return m_texture_id;
}
