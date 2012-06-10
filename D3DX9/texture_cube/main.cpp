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

#include <cmath>
#include <string>
#include <cassert>

#include <d3d9.h>
#include <d3dx9mesh.h>

#include <config_directx_texture_cube.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34

static const FLOAT OBJECT_SIZE = 1;
static const FLOAT ROTATION_DELTA = 0.003f;

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

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pCube = NULL;

LPDIRECT3DTEXTURE9 g_pCurrentTexture = NULL;

LPDIRECT3DTEXTURE9 g_pDirectXTexture = NULL;
LPDIRECT3DTEXTURE9 g_pGamedevTexture = NULL;
LPDIRECT3DTEXTURE9 g_pBoxTexture     = NULL;
LPDIRECT3DTEXTURE9 g_pGobletTexture  = NULL;

static FLOAT g_rotationX = 0;
static FLOAT g_rotationY = 0;
static FLOAT g_rotationZ = 0;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool initTexture();
void setTextureFilter();
bool initGeometry();
void cleanup();

void render();
void camera();
void rotate();

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

    HWND hWnd = CreateWindow(appClass, "D3D9 texture cube",
        WS_OVERLAPPEDWINDOW, 100, 100, 600, 600,
        NULL, NULL, wc.hInstance, NULL);

    if(!init(hWnd)) {
        return 0;
    }

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

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    bool res = initTexture();
    res &= initGeometry();

    return res;
}

bool initTexture()
{
    std::string parentDirectory(PARENT_DIRECTORY + std::string("data/"));

    std::string textureDirectX(parentDirectory + "directx.bmp");
    std::string textureGamedev(parentDirectory + "gamedev.bmp");
    std::string textureBox(parentDirectory + "box.bmp");
    std::string textureGobletBox(parentDirectory + "goblet.bmp");

    D3DXCreateTextureFromFile(g_pd3dDevice, textureDirectX.c_str(), &g_pDirectXTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, textureGamedev.c_str(), &g_pGamedevTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, textureBox.c_str(), &g_pBoxTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, textureGobletBox.c_str(), &g_pGobletTexture);

    if(!g_pDirectXTexture || !g_pGamedevTexture || !g_pGamedevTexture || !g_pGobletTexture) {
        return false;
    }

    g_pd3dDevice->SetTexture(0, g_pDirectXTexture);
    setTextureFilter();

    g_pd3dDevice->SetTexture(0, g_pGamedevTexture);
    setTextureFilter();

    g_pd3dDevice->SetTexture(0, g_pGamedevTexture);
    setTextureFilter();

    g_pd3dDevice->SetTexture(0, g_pGobletTexture);
    setTextureFilter();

    g_pCurrentTexture = g_pDirectXTexture;

    return true;
}

void setTextureFilter()
{
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 4);
}

bool initGeometry()
{
    g_pd3dDevice->CreateVertexBuffer(
        36 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &g_pCube,
        0);

    D3DXVECTOR3 points[8];
    points[0] = D3DXVECTOR3( 1.0f, -1.0f, -1.0f);
    points[1] = D3DXVECTOR3( 1.0f,  1.0f, -1.0f);
    points[2] = D3DXVECTOR3(-1.0f,  1.0f, -1.0f);
    points[3] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
    points[4] = D3DXVECTOR3( 1.0f, -1.0f,  1.0f);
    points[5] = D3DXVECTOR3( 1.0f,  1.0f,  1.0f);
    points[6] = D3DXVECTOR3(-1.0f,  1.0f,  1.0f);
    points[7] = D3DXVECTOR3(-1.0f, -1.0f,  1.0f);

    Vertex* v;
    g_pCube->Lock(0, 0, (void**)&v, 0);

    // front
    v[0] = Vertex(points[1], 1.0f, 1.0f);
    v[1] = Vertex(points[0], 1.0f, 0.0f);
    v[2] = Vertex(points[4], 0.0f, 0.0f);
    v[3] = Vertex(points[1], 1.0f, 1.0f);
    v[4] = Vertex(points[4], 0.0f, 0.0f);
    v[5] = Vertex(points[5], 0.0f, 1.0f);

    // top
    v[6]  = Vertex(points[6], 0.0f, 0.0f);
    v[7]  = Vertex(points[4], 1.0f, 1.0f);
    v[8]  = Vertex(points[7], 1.0f, 0.0f);
    v[9]  = Vertex(points[6], 0.0f, 0.0f);
    v[10] = Vertex(points[5], 0.0f, 1.0f);
    v[11] = Vertex(points[4], 1.0f, 1.0f);

    // back
    v[12] = Vertex(points[6], 0.0f, 0.0f);
    v[13] = Vertex(points[3], 1.0f, 1.0f);
    v[14] = Vertex(points[2], 1.0f, 0.0f);
    v[15] = Vertex(points[6], 0.0f, 0.0f);
    v[16] = Vertex(points[7], 0.0f, 1.0f);
    v[17] = Vertex(points[3], 1.0f, 1.0f);

    // left
    v[18] = Vertex(points[4], 1.0f, 1.0f);
    v[19] = Vertex(points[3], 0.0f, 0.0f);
    v[20] = Vertex(points[7], 0.0f, 1.0f);
    v[21] = Vertex(points[4], 1.0f, 1.0f);
    v[22] = Vertex(points[0], 1.0f, 0.0f);
    v[23] = Vertex(points[3], 0.0f, 0.0f);

    // bottom
    v[24] = Vertex(points[1], 0.0f, 0.0f);
    v[25] = Vertex(points[3], 1.0f, 1.0f);
    v[26] = Vertex(points[0], 1.0f, 0.0f);
    v[27] = Vertex(points[1], 0.0f, 0.0f);
    v[28] = Vertex(points[2], 0.0f, 1.0f);
    v[29] = Vertex(points[3], 1.0f, 1.0f);

    // right
    v[30] = Vertex(points[5], 0.0f, 0.0f);
    v[31] = Vertex(points[2], 1.0f, 1.0f);
    v[32] = Vertex(points[1], 1.0f, 0.0f);
    v[33] = Vertex(points[5], 0.0f, 0.0f);
    v[34] = Vertex(points[6], 0.0f, 1.0f);
    v[35] = Vertex(points[2], 1.0f, 1.0f);

    g_pCube->Unlock();

    return g_pCube ? true : false;
}

void cleanup()
{
    SAFE_RELEASE(g_pDirectXTexture);
    SAFE_RELEASE(g_pGamedevTexture);
    SAFE_RELEASE(g_pBoxTexture);
    SAFE_RELEASE(g_pGobletTexture);

    SAFE_RELEASE(g_pCube);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_pd3dDevice->SetTexture(0, g_pCurrentTexture);

    D3DXMATRIX W;
    D3DXMatrixIdentity(&W);

    D3DXMATRIX X;
    D3DXMatrixRotationX(&X, g_rotationX);
    D3DXMATRIX Y;
    D3DXMatrixRotationY(&Y, g_rotationY);
    D3DXMATRIX Z;
    D3DXMatrixRotationZ(&Z, g_rotationZ);

    W *= X * Y * Z;

    g_pd3dDevice->SetTransform(D3DTS_WORLD, &W);

    g_pd3dDevice->BeginScene();
    {
        g_pd3dDevice->SetStreamSource(0, g_pCube, 0, sizeof(Vertex));
        g_pd3dDevice->SetFVF(Vertex::FVF);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 36);
    }
    g_pd3dDevice->EndScene();

    camera();
    rotate();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void camera()
{
    FLOAT distance = OBJECT_SIZE * 3.0f;
    D3DXVECTOR3 position(distance, distance, distance);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void rotate()
{
    g_rotationX += ROTATION_DELTA;
    g_rotationY += ROTATION_DELTA;
    g_rotationZ += ROTATION_DELTA;
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

    case VK_1:
        g_pCurrentTexture = g_pDirectXTexture;
        break;

    case VK_2:
        g_pCurrentTexture = g_pGamedevTexture;
        break;

    case VK_3:
        g_pCurrentTexture = g_pBoxTexture;
        break;

    case VK_4:
        g_pCurrentTexture = g_pGobletTexture;
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
        //ValidateRect(hWnd, NULL);
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