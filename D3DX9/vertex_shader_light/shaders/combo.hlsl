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

// input variable

matrix ViewWorldMatrix;
matrix ViewProjMatrix;
vector LightDirection;
vector ViewDirection;
vector AmbientMtrl;
vector DiffuseMtrl;
vector SpecularMtrl;
vector AmbientLightInt;
vector DiffuseLightInt;
vector SpecularLightInt;

struct VS_INPUT
{
    vector position : POSITION;
    vector normal   : NORMAL;
};

struct VS_OUTPUT
{
    vector position : POSITION;
    vector color    : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(input.position, ViewProjMatrix);

    vector lightDir = LightDirection;
    vector viewDir = ViewDirection;

    lightDir.w     = 0.0f;
    viewDir.w      = 0.0f;
    input.normal.w = 0.0f;

    lightDir     = mul(lightDir, ViewWorldMatrix);
    viewDir      = mul(viewDir, ViewWorldMatrix);
    input.normal = mul(input.normal, ViewWorldMatrix);

    float s = dot(lightDir, input.normal);
    if(s < 0.0f) {
        s = 0.0f;
    }

    vector H = normalize(lightDir + viewDir);
    float n = 16.0f;
    float D = dot(input.normal, H);

    vector ambient  = AmbientMtrl * AmbientLightInt;
    vector diffuse  = s * (DiffuseMtrl * DiffuseLightInt);
    vector specular = SpecularMtrl * SpecularLightInt * D / (n - D * n + D);

    output.color = ambient + diffuse + specular;
    return output;
}