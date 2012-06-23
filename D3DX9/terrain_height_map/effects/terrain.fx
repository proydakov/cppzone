/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

uniform extern matrix  ViewProj;
uniform extern vector  DirToSunW;
uniform extern texture GrassTex;
uniform extern texture DirtTex;
uniform extern texture StoneTex;
uniform extern texture BlendMap;

uniform extern bool DrawBlendMap;

static float TexScale     = 24.0f;
static float AmbientLight = 0.3f;

///////////////////////////////////////////////////////////////////////////////

sampler Tex0S = sampler_state
{
    Texture   = <GrassTex>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler Tex1S = sampler_state
{
    Texture   = <DirtTex>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler Tex2S = sampler_state
{
    Texture   = <StoneTex>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler BlendMapS = sampler_state
{
    Texture   = <BlendMap>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

///////////////////////////////////////////////////////////////////////////////

struct VS_INPUT
{
    vector position : POSITION0;
    vector normalW  : NORMAL0;
    vector tex0     : TEXCOORD0;
};

struct VS_OUTPUT
{
    vector position     : POSITION0;
    float2 nonTiledTex  : TEXCOORD0;
    //float2 tiledTexC    : TEXCOORD1;
    float  shade        : TEXCOORD2;
};

VS_OUTPUT TerrainVS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.shade = saturate(max(0.0f, dot(input.normalW, DirToSunW)) + AmbientLight);

    input.position.w = 1.0f;
	output.position = mul(input.position, ViewProj);

    //output.tiledTexC    = input.tex0 * TexScale;
    output.nonTiledTex = input.tex0;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct PS_INPUT
{
    float2 nonTiledTex  : TEXCOORD0;
    //float2 tiledTexC    : TEXCOORD0;
    float shade         : TEXCOORD2;
};

struct PS_OUTPUT
{
    vector color : COLOR0;
};

PS_OUTPUT TerrainPS(PS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT)0;

    //vector c0 = tex2D(Tex0S, input.tiledTexC);
    //vector c1 = tex2D(Tex1S, input.tiledTexC);
    //vector c2 = tex2D(Tex2S, input.tiledTexC);

    if(!DrawBlendMap) {
        vector c0 = tex2D(Tex0S, input.nonTiledTex);
        vector c1 = tex2D(Tex1S, input.nonTiledTex);
        vector c2 = tex2D(Tex2S, input.nonTiledTex);

        vector B = tex2D(BlendMapS, input.nonTiledTex);

        float inverse = 1.0f / (B.r + B.g + B.b);

        c0 *= B.r * inverse;
        c1 *= B.g * inverse;
        c2 *= B.b * inverse;

        output.color = (c0 + c1 + c2) * input.shade;
    }
    else {
        output.color = tex2D(BlendMapS, input.nonTiledTex);
    }
    return output;
}

///////////////////////////////////////////////////////////////////////////////

technique TerrainTech
{
    pass P0
    {
        vertexShader = compile vs_3_0 TerrainVS();
        pixelShader  = compile ps_3_0 TerrainPS();
    }
}
