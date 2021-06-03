#ifdef GL_ES
precision mediump float;
#endif

varying vec2 fTexCoord;
uniform sampler2D fTexture0;
uniform sampler2D fTexture1;

void main()
{
    vec4 baseColor = texture2D( fTexture0, fTexCoord );
    vec4 secondColor = texture2D( fTexture1, fTexCoord );
    gl_FragColor = baseColor * secondColor;
}
