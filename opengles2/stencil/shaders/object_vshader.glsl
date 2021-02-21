#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 vPosition;
attribute vec2 vTexCoord;

uniform mat4 vVPMatrix;
uniform mat4 vModelMatrix;

varying vec2 fTexCoord;

void main()
{
    gl_Position = vVPMatrix * vModelMatrix * vPosition;
    fTexCoord = vTexCoord;
}
