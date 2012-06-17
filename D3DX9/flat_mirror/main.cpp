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

#include <config_directx_flat_mirror.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

struct Vertex
{
    Vertex(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fu, FLOAT fv) :
        x(fx), y(fy), z(fz),
        u(fu), v(fv)
        {
        }

    Vertex(D3DXVECTOR3 points, FLOAT fu, FLOAT fv) :
        x(points.x), y(points.y), z(points.z),
        u(fu), v(fv)
        {
        }

    FLOAT x, y, z;
    FLOAT u, v;
    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

static const FLOAT MIRROR_SIZE = 10.0f;
static const FLOAT MAX_ROTATION = D3DX_PI * 2;
static const FLOAT ROTATION_DELTA = 0.01f;
static const FLOAT MOVE_DELTA = 0.05f;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pMirror   = NULL;
LPDIRECT3DTEXTURE9 g_pMirrotTexture = NULL;
D3DXPLANE g_MirrotPlane = D3DXPLANE(0, 0, 1, 0);

LPD3DXMESH  g_pTeapotMesh           = NULL;
LPDIRECT3DTEXTURE9 g_pTeapotTexture = NULL;
D3DXVECTOR3 g_TeapotPosition = D3DXVECTOR3(MIRROR_SIZE / 2, MIRROR_SIZE / 2, -MIRROR_SIZE / 2);

FLOAT g_rotate = 0;
FLOAT g_move = MOVE_DELTA;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool init_texture();
void set_texture_filter();
bool init_object();
void cleanup();

void render();
void camera();
void rotate();
void move();

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

    HWND hWnd = CreateWindow(appClass, "D3D9 falt mirror",
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

    bool res = init_texture();
    res &= init_object();

    return res;
}

bool init_texture()
{
    std::string parentDirectory(TEXTURE_DIRECTORY);
    std::string textureMirror(parentDirectory + "mirror.tga");
    std::string textureTeapot(parentDirectory + "teapot.tga");

    D3DXCreateTextureFromFile(g_pd3dDevice, textureMirror.c_str(), &g_pMirrotTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, textureTeapot.c_str(), &g_pTeapotTexture);

    if(!g_pMirrotTexture || !g_pTeapotTexture) {
        return false;
    }

    g_pd3dDevice->SetTexture(0, g_pMirrotTexture);
    set_texture_filter();

    g_pd3dDevice->SetTexture(0, g_pTeapotTexture);
    set_texture_filter();

    return true;
}

void set_texture_filter()
{
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
}

bool init_object()
{
    // init mirror
    g_pd3dDevice->CreateVertexBuffer(
        6 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &g_pMirror,
        0);

    D3DXVECTOR3 points[8];
    points[0] = D3DXVECTOR3( 0.0f,        0.0f,        0.0f);
    points[1] = D3DXVECTOR3( MIRROR_SIZE, 0.0f,        0.0f);
    points[2] = D3DXVECTOR3( MIRROR_SIZE, MIRROR_SIZE, 0.0f);
    points[3] = D3DXVECTOR3( 0.0f,        MIRROR_SIZE, 0.0f);

    Vertex* v;
    g_pMirror->Lock(0, 0, (void**)&v, 0);

    v[0] = Vertex(points[1], 1.0f, 1.0f);
    v[1] = Vertex(points[3], 0.0f, 0.0f);
    v[2] = Vertex(points[0], 1.0f, 0.0f);
    v[3] = Vertex(points[1], 1.0f, 1.0f);
    v[4] = Vertex(points[2], 0.0f, 1.0f);
    v[5] = Vertex(points[3], 0.0f, 0.0f);

    g_pMirror->Unlock();

    if(!g_pMirror) {
        return false;
    }

    // init teapot
    D3DXCreateTeapot(g_pd3dDevice, &g_pTeapotMesh, 0);
    return g_pTeapotMesh ? true : false;
}

void cleanup()
{
    SAFE_RELEASE(g_pTeapotMesh);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_pd3dDevice->BeginScene();
    {
        D3DXMATRIX MWORLD;

        // draw teapot
        g_pd3dDevice->SetTexture(0, g_pTeapotTexture);
        g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        D3DXMATRIX MTRANS;
        D3DXMatrixTranslation(&MTRANS, g_TeapotPosition.x, g_TeapotPosition.y, g_TeapotPosition.z);
        D3DXMATRIX MROTATE;
        D3DXMatrixRotationY(&MROTATE, g_rotate);

        MWORLD = MROTATE * MTRANS;
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);

        g_pTeapotMesh->DrawSubset(0);

        // draw mirror
        g_pd3dDevice->SetTexture(0, g_pMirrotTexture);
        g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

        D3DXMatrixIdentity(&MWORLD);
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);

        g_pd3dDevice->SetStreamSource(0, g_pMirror, 0, sizeof(Vertex));
        g_pd3dDevice->SetFVF(Vertex::FVF);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 6);

        g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILREF, 0x1);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

        g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
        g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 6);

        g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

        //draw reflection
        g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

        g_pd3dDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
        g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

        D3DXMATRIX MREF;
        D3DXMatrixReflect(&MREF, &g_MirrotPlane);
        MWORLD = MROTATE * MTRANS * MREF;
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);

        g_pd3dDevice->SetTexture(0, g_pTeapotTexture);
        g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
        g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

        g_pTeapotMesh->DrawSubset(0);

        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
    rotate();
    move();
}

void camera()
{
    D3DXVECTOR3 position(MIRROR_SIZE * 3 / 4, MIRROR_SIZE * 3 / 4, -MIRROR_SIZE * 1.5f);
    D3DXVECTOR3 target(MIRROR_SIZE / 2, MIRROR_SIZE / 2, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void rotate()
{
    g_rotate += ROTATION_DELTA;
    if(g_rotate > MAX_ROTATION) {
        g_rotate -= MAX_ROTATION;
    }
}

void move()
{
    if(g_TeapotPosition.x > MIRROR_SIZE * 11 / 10) {
        g_move = - MOVE_DELTA;
    }
    else if(g_TeapotPosition.x < 0.0f - MIRROR_SIZE / 10) {
        g_move = MOVE_DELTA;
    }
    g_TeapotPosition.x += g_move;
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