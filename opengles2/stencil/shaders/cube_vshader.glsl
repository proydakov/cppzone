#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 vPosition;
attribute vec2 vTexCoord;

uniform mat4 vMvpMatrix;

varying vec2 fTexCoord;

void main()
{
    gl_Position = vMvpMatrix * vPosition;
    fTexCoord = vTexCoord;
}
