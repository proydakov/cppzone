#include "opengles2_sdk/opengles2_texture.h"

opengles2_texture::opengles2_texture() :
    m_texture_id(0)
{
}

opengles2_texture::~opengles2_texture()
{
    unload();
}

bool opengles2_texture::load(int width, int height, GLvoid* pixels)
{
    unload();

    // Use tightly packed data
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

    // Generate a texture object
    glGenTextures ( 1, &m_texture_id );

    // Bind the texture object
    glBindTexture ( GL_TEXTURE_2D, m_texture_id );

    // Load the texture
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

    // Set the filtering mode
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    return true;
}

void opengles2_texture::unload()
{
    if(m_texture_id) {
        glDeleteTextures ( 1, &m_texture_id );
        m_texture_id = 0;
    }
}

GLuint opengles2_texture::get_id() const
{
    return m_texture_id;
}
