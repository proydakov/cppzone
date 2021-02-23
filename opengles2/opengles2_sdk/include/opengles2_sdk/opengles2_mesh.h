#pragma once

#include "opengles2.h"

struct opengles2_mesh
{
    using GLfloatArray72 = GLfloat[72];
    using GLfloatArray48 = GLfloat[48];
    using GLushort36 = GLushort[36];

    static const GLfloatArray72& cubeVertices() noexcept;
    static const GLfloatArray48& cubeTextures() noexcept;
    static const GLushort36& cubeIndices() noexcept;

    static GLsizei cubeIndicesSize() noexcept;
};
