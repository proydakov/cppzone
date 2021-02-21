#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 vPosition;
attribute vec4 vNormal;

uniform mat4 vVPMatrix;
uniform mat4 vModelMatrix;

void main()
{
    vec4 vector = vec4(vPosition.x + vNormal.x * 0.1, vPosition.y + vNormal.y * 0.1, vPosition.z + vNormal.z * 0.1, vPosition.a);
    gl_Position = vVPMatrix * vModelMatrix * vector;
}
