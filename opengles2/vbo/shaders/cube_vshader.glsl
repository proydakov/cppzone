#ifdef GL_ES
precision mediump float;
#endif

attribute vec3 vPosition;
attribute vec2 vTexCoord;

uniform mat4 vMvpMatrix;

varying vec2 fTexCoord;

void main()
{
    gl_Position = vMvpMatrix * vec4(vPosition.x, vPosition.y, vPosition.z, 1);
    fTexCoord = vTexCoord;
}
