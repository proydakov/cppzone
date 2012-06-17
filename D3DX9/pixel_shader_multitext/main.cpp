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

#include <string>
#include <cassert>

#include <d3d9.h>
#include <d3dx9mesh.h>

#include <config_pixel_shader_multitext.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

struct MultiTexVertex
{
    MultiTexVertex(D3DXVECTOR3& iposition, D3DXVECTOR2& itex1, D3DXVECTOR2& itex2, D3DXVECTOR2& itex3) :
        position(iposition),
        texture1(itex1),
        texture2(itex2),
        texture3(itex3)
        {
        }

    D3DXVECTOR3 position;
    D3DXVECTOR2 texture1;
    D3DXVECTOR2 texture2;
    D3DXVECTOR2 texture3;

    static const DWORD FVF;
};
const DWORD MultiTexVertex::FVF = D3DFVF_XYZ | D3DFVF_TEX3;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pQuadVB = NULL;

//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pBaseTex   = NULL;
LPDIRECT3DTEXTURE9 g_pLightTex  = NULL;
LPDIRECT3DTEXTURE9 g_pStringTex = NULL;

//-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE g_pVertexShaderConstantTable = NULL;
LPDIRECT3DVERTEXSHADER9 g_pVertexShader          = NULL;
D3DXHANDLE g_vertexShaderViewProjMatrixHandle = 0;

LPDIRECT3DVERTEXSHADER9 g_pCurrentVS = NULL;
LPDIRECT3DVERTEXSHADER9 g_pLastVS    = NULL;

//-----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 g_pMultiTexPS = NULL;
LPD3DXCONSTANTTABLE    g_pMultiTexCT = NULL;
D3DXHANDLE g_baseTexHandle   = 0;
D3DXHANDLE g_lightTexHandle  = 0;
D3DXHANDLE g_stringTexHandle = 0;
D3DXCONSTANT_DESC g_baseTexDesc;
D3DXCONSTANT_DESC g_lightTexDesc;
D3DXCONSTANT_DESC g_stringTexDesc;

LPDIRECT3DPIXELSHADER9 g_pCurrentPS = NULL;
LPDIRECT3DPIXELSHADER9 g_pLastPS    = NULL;
//-----------------------------------------------------------------------------

D3DXMATRIX g_viewMatrix;
D3DXMATRIX g_projMatrix;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool init_textures();
bool init_object();
bool init_shaders();
bool init_vertex_shader(const std::string& shader_dir);
bool init_pixel_shader(const std::string& shader_dir);
void cleanup();

void render();
void config_vertex_shader();
void config_pixel_shader();
void camera();

void resize(UINT width, UINT height);
void keyboard(HWND hWnd, WPARAM key);
LRESULT WINAPI msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
    UNREFERENCED_PARAMETER(hInst);

    CHAR appClass[] = "D3D9 Examples";

    WNDCLASSEX wc =
    {
        sizeof(WNDCLASSEX), CS_CLASSDC, msgProc, 0L, 0L,
        GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
        appClass, NULL
    };
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow(appClass, "D3D9 pixel shader multitexturing",
        WS_OVERLAPPEDWINDOW, 100, 100, 600, 600,
        NULL, NULL, wc.hInstance, NULL);

    bool res = init(hWnd);
    assert(res);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(appClass, wc.hInstance);
    return 0;
}

bool init(HWND hWnd)
{
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if(!g_pD3D) {
        return false;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

    if(!g_pd3dDevice) {
        return false;
    }

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    bool res = true;

    res &= init_textures();
    res &= init_object();
    res &= init_shaders();

    return true;
}

bool init_textures()
{
    std::string baseDirectory(TEXTURES_DIRECTORY);
    std::string baseTex(baseDirectory + "box.tga");
    std::string lightTex(baseDirectory + "circle_light.tga");
    std::string stringTex(baseDirectory + "multitexturing_text.tga");

    D3DXCreateTextureFromFile(g_pd3dDevice, baseTex.c_str(), &g_pBaseTex);
    D3DXCreateTextureFromFile(g_pd3dDevice, lightTex.c_str(), &g_pLightTex);
    D3DXCreateTextureFromFile(g_pd3dDevice, stringTex.c_str(), &g_pStringTex);

    if(!g_pBaseTex || !g_pLightTex || !g_pStringTex) {
        return false;
    }
    return true;
}

bool init_object()
{
    g_pd3dDevice->CreateVertexBuffer(
        6 * sizeof(MultiTexVertex),
        D3DUSAGE_WRITEONLY,
        MultiTexVertex::FVF,
        D3DPOOL_MANAGED,
        &g_pQuadVB,
        NULL);

    D3DXVECTOR3 points[4];
    points[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    points[1] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    points[2] = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
    points[3] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    D3DXVECTOR2 texture[4];
    texture[0] = D3DXVECTOR2(0.0f, 0.0f);
    texture[1] = D3DXVECTOR2(1.0f, 0.0f);
    texture[2] = D3DXVECTOR2(1.0f, 1.0f);
    texture[3] = D3DXVECTOR2(0.0f, 1.0f);

    MultiTexVertex* v;
    g_pQuadVB->Lock(0, 0, (void**)&v, 0);

    v[0] = MultiTexVertex(points[1], texture[1], texture[1], texture[2]);//
    v[1] = MultiTexVertex(points[3], texture[3], texture[3], texture[0]);//
    v[2] = MultiTexVertex(points[0], texture[0], texture[0], texture[3]);//
    v[3] = MultiTexVertex(points[1], texture[1], texture[1], texture[2]);//
    v[4] = MultiTexVertex(points[2], texture[2], texture[2], texture[1]);//
    v[5] = MultiTexVertex(points[3], texture[3], texture[3], texture[0]);//

    g_pQuadVB->Unlock();

    return g_pQuadVB ? true : false;
}

bool init_shaders()
{
    std::string base_dir(SHADER_DIRECTORY);

    bool res = true;
    res &= init_vertex_shader(base_dir);
    res &= init_pixel_shader(base_dir);

    g_pCurrentPS = g_pMultiTexPS;
    g_pCurrentVS = g_pVertexShader;

    return res;
}

bool init_vertex_shader(const std::string& shader_dir)
{
    std::string shader_name("vertex_transform.hlsl");
    std::string shader_file(shader_dir + shader_name);

    ID3DXBuffer* pShader = 0;
    ID3DXBuffer* pErrorBuffer = 0;

    D3DXCompileShaderFromFile (
        shader_file.c_str(),
        NULL,
        NULL,
        "main",
        "vs_3_0",
        D3DXSHADER_DEBUG,
        &pShader,
        &pErrorBuffer,
        &g_pVertexShaderConstantTable);

    if(pErrorBuffer) {
        MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(pErrorBuffer);
    }
    if(!pShader) {
        return false;
    }

    g_pd3dDevice->CreateVertexShader(
        (DWORD*)pShader->GetBufferPointer(),
        &g_pVertexShader);

    SAFE_RELEASE(pShader);

    if(!g_pVertexShader) {
        return false;
    }

    g_vertexShaderViewProjMatrixHandle = g_pVertexShaderConstantTable->GetConstantByName(0, "ViewProjMatrix");

    g_pVertexShaderConstantTable->SetDefaults(g_pd3dDevice);

    return true;
}

bool init_pixel_shader(const std::string& shader_dir)
{
    std::string shaderName(shader_dir + "multitexturing.hlsl");

    ID3DXBuffer* shader = 0;
    ID3DXBuffer* errorBuffer = 0;

    D3DXCompileShaderFromFile(
        shaderName.c_str(),
        NULL,
        NULL,
        "main",
        "ps_3_0",
        D3DXSHADER_DEBUG,
        &shader,
        &errorBuffer,
        &g_pMultiTexCT);

    if(errorBuffer) {
        MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(errorBuffer);
        return false;
    }

    g_pd3dDevice->CreatePixelShader(
        (DWORD*)shader->GetBufferPointer(),
        &g_pMultiTexPS);

    SAFE_RELEASE(shader);

    if(!g_pMultiTexPS) {
        MessageBox(0, "CreateVertexShader - FAILED", 0, 0);
        return false;
    }

    g_baseTexHandle   = g_pMultiTexCT->GetConstantByName(0, "BaseTex");
    g_lightTexHandle  = g_pMultiTexCT->GetConstantByName(0, "LightTex");
    g_stringTexHandle = g_pMultiTexCT->GetConstantByName(0, "StringTex");

    UINT count;
    g_pMultiTexCT->GetConstantDesc(
        g_baseTexHandle,
        &g_baseTexDesc,
        &count);

    g_pMultiTexCT->GetConstantDesc(
        g_stringTexHandle,
        &g_stringTexDesc,
        &count);

    g_pMultiTexCT->GetConstantDesc(
        g_lightTexHandle,
        &g_lightTexDesc,
        &count);

    g_pMultiTexCT->SetDefaults(g_pd3dDevice);

    return true;
}

void cleanup()
{
    SAFE_RELEASE(g_pQuadVB);

    SAFE_RELEASE(g_pBaseTex);
    SAFE_RELEASE(g_pLightTex);
    SAFE_RELEASE(g_pStringTex);

    SAFE_RELEASE(g_pMultiTexCT);
    SAFE_RELEASE(g_pMultiTexPS);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(125, 125, 125), 1.0f, 0);

    g_pd3dDevice->BeginScene();
    {
        D3DXMATRIX ViewWorld = g_viewMatrix;
        D3DXMATRIX ViewProj = ViewWorld * g_projMatrix;

        g_pVertexShaderConstantTable->SetMatrix(g_pd3dDevice, g_vertexShaderViewProjMatrixHandle, &ViewProj);

        if(g_pLastVS != g_pCurrentVS) {
            config_vertex_shader();
            g_pd3dDevice->SetVertexShader(g_pCurrentVS);
            g_pLastVS = g_pCurrentVS;
        }

        if(g_pLastPS != g_pCurrentPS) {
            config_pixel_shader();
            g_pd3dDevice->SetPixelShader(g_pCurrentPS);
            g_pLastPS = g_pCurrentPS;
        }

        g_pd3dDevice->SetStreamSource(0, g_pQuadVB, 0, sizeof(MultiTexVertex));
        g_pd3dDevice->SetFVF(MultiTexVertex::FVF);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 6);
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
}

void config_vertex_shader()
{
}

void config_pixel_shader()
{
    g_pd3dDevice->SetTexture(g_baseTexDesc.RegisterIndex, g_pBaseTex);
    g_pd3dDevice->SetSamplerState(g_baseTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_baseTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_baseTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

    g_pd3dDevice->SetTexture(g_lightTexDesc.RegisterIndex, g_pLightTex);
    g_pd3dDevice->SetSamplerState(g_lightTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_lightTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_lightTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

    g_pd3dDevice->SetTexture(g_stringTexDesc.RegisterIndex, g_pStringTex);
    g_pd3dDevice->SetSamplerState(g_stringTexDesc.RegisterIndex, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_stringTexDesc.RegisterIndex, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(g_stringTexDesc.RegisterIndex, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
}

void camera()
{
    D3DXVECTOR3 pos(0.5f, 0.5f, -2.0f);
    D3DXVECTOR3 target(0.5f, 0.5f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &pos, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);

    g_viewMatrix = V;
}

void resize(UINT width, UINT height)
{
    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
        &proj,
        D3DX_PI * 0.25f,
        (FLOAT) width / (FLOAT) height,
        0.1f,
        100.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);

    g_projMatrix = proj;
}

void keyboard(HWND hWnd, WPARAM key)
{
    switch(key) {
    case VK_ESCAPE:
        DestroyWindow(hWnd);
        break;
    }
}

LRESULT WINAPI msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg) {
    case WM_DESTROY:
        cleanup();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        render();
        return 0;

    case WM_SIZE:
        RECT rect;
        GetClientRect(hWnd, &rect);
        resize(rect.right - rect.left, rect.bottom - rect.top);
        break;

    case WM_KEYDOWN:
        keyboard(hWnd, wParam);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}