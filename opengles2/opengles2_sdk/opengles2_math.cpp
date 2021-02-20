#include <cmath>
#include <cstring>

#include "opengles2_sdk/opengles2_math.h"

#define PI 3.1415926535897932384626433832795f

void opengles2_matrix::scale(opengles2_matrix& result, GLfloat sx, GLfloat sy, GLfloat sz) noexcept
{
    result.m[0][0] *= sx;
    result.m[0][1] *= sx;
    result.m[0][2] *= sx;
    result.m[0][3] *= sx;

    result.m[1][0] *= sy;
    result.m[1][1] *= sy;
    result.m[1][2] *= sy;
    result.m[1][3] *= sy;

    result.m[2][0] *= sz;
    result.m[2][1] *= sz;
    result.m[2][2] *= sz;
    result.m[2][3] *= sz;
}

void opengles2_matrix::translate(opengles2_matrix& result, GLfloat tx, GLfloat ty, GLfloat tz) noexcept
{
    result.m[3][0] += (result.m[0][0] * tx + result.m[1][0] * ty + result.m[2][0] * tz);
    result.m[3][1] += (result.m[0][1] * tx + result.m[1][1] * ty + result.m[2][1] * tz);
    result.m[3][2] += (result.m[0][2] * tx + result.m[1][2] * ty + result.m[2][2] * tz);
    result.m[3][3] += (result.m[0][3] * tx + result.m[1][3] * ty + result.m[2][3] * tz);
}

void opengles2_matrix::rotate(opengles2_matrix& result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z) noexcept
{
    GLfloat mag = sqrtf(x * x + y * y + z * z);

    GLfloat sinAngle = sinf ( angle * PI / 180.0f );
    GLfloat cosAngle = cosf ( angle * PI / 180.0f );

    if ( mag > 0.0f )
    {
        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLfloat oneMinusCos;
        opengles2_matrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0F;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0F;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0F;

        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        opengles2_matrix::multiply(result, rotMat, result);
   }
}

void opengles2_matrix::frustum(opengles2_matrix& result, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) noexcept
{
    GLfloat deltaX = right - left;
    GLfloat deltaY = top - bottom;
    GLfloat deltaZ = farZ - nearZ;

    if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
         (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
         return;

    opengles2_matrix frust;

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    opengles2_matrix::multiply(result, frust, result);
}

void opengles2_matrix::perspective(opengles2_matrix& result, GLfloat fovy, GLfloat aspect, GLfloat nearZ, GLfloat farZ) noexcept
{
   GLfloat frustumW, frustumH;

   frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
   frustumW = frustumH * aspect;

   opengles2_matrix::frustum( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

void opengles2_matrix::ortho(opengles2_matrix& result, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearZ, GLfloat farZ) noexcept
{
    GLfloat deltaX = right - left;
    GLfloat deltaY = top - bottom;
    GLfloat deltaZ = farZ - nearZ;

    if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
        return;

    opengles2_matrix ortho;
    opengles2_matrix::load_identity(ortho);

    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

    opengles2_matrix::multiply(result, ortho, result);
}

void opengles2_matrix::multiply(opengles2_matrix& result, opengles2_matrix& srcA, opengles2_matrix& srcB) noexcept
{
    opengles2_matrix tmp;

    for (int i = 0; i < 4; i++)
    {
        tmp.m[i][0] =   (srcA.m[i][0] * srcB.m[0][0]) +
                        (srcA.m[i][1] * srcB.m[1][0]) +
                        (srcA.m[i][2] * srcB.m[2][0]) +
                        (srcA.m[i][3] * srcB.m[3][0]) ;

        tmp.m[i][1] =   (srcA.m[i][0] * srcB.m[0][1]) +
                        (srcA.m[i][1] * srcB.m[1][1]) +
                        (srcA.m[i][2] * srcB.m[2][1]) +
                        (srcA.m[i][3] * srcB.m[3][1]) ;

        tmp.m[i][2] =   (srcA.m[i][0] * srcB.m[0][2]) +
                        (srcA.m[i][1] * srcB.m[1][2]) +
                        (srcA.m[i][2] * srcB.m[2][2]) +
                        (srcA.m[i][3] * srcB.m[3][2]) ;

        tmp.m[i][3] =   (srcA.m[i][0] * srcB.m[0][3]) +
                        (srcA.m[i][1] * srcB.m[1][3]) +
                        (srcA.m[i][2] * srcB.m[2][3]) +
                        (srcA.m[i][3] * srcB.m[3][3]) ;
    }
    memcpy(&result, &tmp, sizeof(opengles2_matrix));
}

void opengles2_matrix::load_identity(opengles2_matrix& result) noexcept
{
    memset(&result, 0x0, sizeof(opengles2_matrix));
    result.m[0][0] = 1.0f;
    result.m[1][1] = 1.0f;
    result.m[2][2] = 1.0f;
    result.m[3][3] = 1.0f;
}
