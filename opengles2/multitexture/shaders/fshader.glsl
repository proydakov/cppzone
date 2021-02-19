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

/*

    vec4 baseColor = texture2D( fTexture0, fTexCoord );
    vec4 lightColor = texture2D( fTexture1, fTexCoord );
    if (baseColor.b < 0.1)
    {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 0);
    }
    else
    {
        gl_FragColor = baseColor;
    }

*/
