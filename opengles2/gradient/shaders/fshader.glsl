#ifdef GL_ES
precision mediump float;
#endif

varying vec4 fColor;

void main()
{
    gl_FragColor = vec4(fColor.x, fColor.y, fColor.z, fColor.a);
}
