#pragma once

#include "opengles2.h"

struct opengles2_matrix
{
    GLfloat m[4][4];

    //
    /// \brief a simple way to pass matrix into glUniformMatrix4fv
    //
    GLfloat const* toUniform() const noexcept
    {
        return &m[0][0];
    }

    //
    /// \brief multiply matrix specified by result with a scaling matrix and return new matrix in result
    /// \param result Specifies the input matrix.  Scaled matrix is returned in result.
    /// \param sx, sy, sz Scale factors along the x, y and z axes respectively
    //
    static void scale(opengles2_matrix& result, GLfloat sx, GLfloat sy, GLfloat sz) noexcept;

    //
    /// \brief multiply matrix specified by result with a translation matrix and return new matrix in result
    /// \param result Specifies the input matrix.  Translated matrix is returned in result.
    /// \param tx, ty, tz Scale factors along the x, y and z axes respectively
    //
    static void translate(opengles2_matrix& result, GLfloat tx, GLfloat ty, GLfloat tz) noexcept;

    //
    /// \brief multiply matrix specified by result with a rotation matrix and return new matrix in result
    /// \param result Specifies the input matrix.  Rotated matrix is returned in result.
    /// \param angle Specifies the angle of rotation, in degrees.
    /// \param x, y, z Specify the x, y and z coordinates of a vector, respectively
    //
    static void rotate(opengles2_matrix& result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) noexcept;

    //
    // \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
    /// \param result Specifies the input matrix.  new matrix is returned in result.
    /// \param left, right Coordinates for the left and right vertical clipping planes
    /// \param bottom, top Coordinates for the bottom and top horizontal clipping planes
    /// \param nearZ, farZ Distances to the near and far depth clipping planes.  Both distances must be positive.
    //
    static void frustum(opengles2_matrix& result, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) noexcept;

    //
    /// \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
    /// \param result Specifies the input matrix.  new matrix is returned in result.
    /// \param fovy Field of view y angle in degrees
    /// \param aspect Aspect ratio of screen
    /// \param nearZ Near plane distance
    /// \param farZ Far plane distance
    //
    static void perspective(opengles2_matrix& result, GLfloat fovy, GLfloat aspect, GLfloat nearZ, GLfloat farZ) noexcept;

    //
    /// \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
    /// \param result Specifies the input matrix.  new matrix is returned in result.
    /// \param left, right Coordinates for the left and right vertical clipping planes
    /// \param bottom, top Coordinates for the bottom and top horizontal clipping planes
    /// \param nearZ, farZ Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
    //
    static void ortho(opengles2_matrix& result, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) noexcept;

    //
    /// \brief perform the following operation - result matrix = srcA matrix * srcB matrix
    /// \param result Returns multiplied matrix
    /// \param srcA, srcB Input matrices to be multiplied
    //
    static void multiply(opengles2_matrix& result, opengles2_matrix& srcA, opengles2_matrix& srcB) noexcept;

    //
    //// \brief return an indentity matrix
    //// \param result returns identity matrix
    //
    static void load_identity(opengles2_matrix& result) noexcept;
};
