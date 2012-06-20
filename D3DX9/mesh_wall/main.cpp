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

#pragma warning (disable:4996)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#include <config_mesh_wall.h>

struct Vertex
{
    Vertex(const D3DXVECTOR3& iposition, const D3DXVECTOR2& itext1) :
        position(iposition),
        normal(D3DXVECTOR3(0, 0, 0)),
        text1(itext1)
    {
    }

    Vertex(const D3DXVECTOR3& iposition, const D3DXVECTOR3& inormal, const D3DXVECTOR2& itext1) :
        position(iposition),
        normal(inormal),
        text1(itext1)
    {
    }

    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 text1;

    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPDIRECT3DTEXTURE9 g_pWallTexture = NULL;
LPD3DXMESH g_pMeshWall = NULL;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool init_texture();
bool init_wall();
void cleanup();

void render();
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

    HWND hWnd = CreateWindow(appClass, "D3D9 mesh wall",
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
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

    if(!g_pd3dDevice) {
        return false;
    }
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    bool res = true;
    res &= init_texture();
    res &= init_wall();

    return res;
}

bool init_texture()
{
    std::string parent_dir(TEXTURES_DIRECTORY);

    std::string texture_wall(parent_dir + "wall.png");
    D3DXCreateTextureFromFile(g_pd3dDevice, texture_wall.c_str(), &g_pWallTexture);
    g_pd3dDevice->SetTexture(0, g_pWallTexture);

    if(!g_pWallTexture) {
        return false;
    }
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

    return true;
}

bool init_wall()
{
    size_t num_faces = 2;
    size_t num_vertex = 4;

    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    D3DXDeclaratorFromFVF(Vertex::FVF, elems);
    D3DXCreateMesh(num_faces, num_vertex, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT, elems, g_pd3dDevice, &g_pMeshWall);

    ///////////////////////////////////////////////////////////////////////////

    Vertex* vertex = 0;
    g_pMeshWall->LockVertexBuffer(0,(void**)&vertex);

    vertex[0].position = D3DXVECTOR3(0, 0, 0);
    vertex[0].text1    = D3DXVECTOR2(1, 1);

    vertex[1].position = D3DXVECTOR3(0, 1, 0);
    vertex[1].text1    = D3DXVECTOR2(1, 0);
    
    vertex[2].position = D3DXVECTOR3(1, 0, 0);
    vertex[2].text1    = D3DXVECTOR2(0, 1);
    
    vertex[3].position = D3DXVECTOR3(1, 1, 0);
    vertex[3].text1    = D3DXVECTOR2(0, 0);

    g_pMeshWall->UnlockVertexBuffer();

    ///////////////////////////////////////////////////////////////////////////

    DWORD* index = 0;
    g_pMeshWall->LockIndexBuffer(0, (void**)&index);

    index[0] = 1;
    index[1] = 3;
    index[2] = 0;
    index[3] = 3;
    index[4] = 2;
    index[5] = 0;

    g_pMeshWall->UnlockIndexBuffer();

    ///////////////////////////////////////////////////////////////////////////

    DWORD* attBuffer = 0;
    g_pMeshWall->LockAttributeBuffer(0, &attBuffer);

    for(size_t i = 0; i < num_faces; ++i) {
        attBuffer[i] = 0;
    }

    g_pMeshWall->UnlockAttributeBuffer();

    ///////////////////////////////////////////////////////////////////////////

    D3DXComputeNormals(g_pMeshWall, NULL);

    return true;
}

void cleanup()
{
    SAFE_RELEASE(g_pWallTexture);
    SAFE_RELEASE(g_pMeshWall);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_pd3dDevice->BeginScene();
    {
        g_pd3dDevice->SetTexture(0, g_pWallTexture);
        g_pMeshWall->DrawSubset(0);
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
}

void camera()
{
    D3DXVECTOR3 position(0.5f, 0.5f, -2.0f);
    D3DXVECTOR3 target(0.5f, 0.5f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
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

#pragma warning (default:4996)