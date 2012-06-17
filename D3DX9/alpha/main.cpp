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
#include <cassert>

#include <d3d9.h>
#include <d3dx9math.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

static const FLOAT OBJECT_SIZE = 1;

struct Vertex
{
    Vertex(FLOAT vx, FLOAT vy, FLOAT vz, D3DCOLOR& vcolor) :
        x(vx), y(vy), z(vz), color(vcolor)
    {
    }

    FLOAT x, y, z;
    D3DCOLOR color;
    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPDIRECT3DVERTEXBUFFER9 g_pLeftTriangle  = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pRightTriangle = NULL;

bool g_leftObjectTop = true;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool init_object();
void cleanup();

void render();
void camera();
void drawLeftTriangle();
void drawRightTriangle();

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

    HWND hWnd = CreateWindow(appClass, "D3D9 alpha",
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
    
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    bool res = init_object();

    return res;
}

bool init_object()
{
    DWORD alpha = 255 * 0.75f;

    D3DCOLOR blue = D3DCOLOR_ARGB(alpha, 0, 0, 255);
    D3DCOLOR red  = D3DCOLOR_ARGB(alpha, 255, 0, 0);

    // create left triangle
    g_pd3dDevice->CreateVertexBuffer(
        3 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &g_pLeftTriangle,
        0);

    Vertex* v1;
    g_pLeftTriangle->Lock(0, 0, (void**)&v1, 0);

    v1[0] = Vertex(-1.0f,  1.0f, 0.0f, blue);
    v1[1] = Vertex( 1.0f, -1.0f, 0.0f, blue);
    v1[2] = Vertex(-1.0f, -1.0f, 0.0f, blue);

    g_pLeftTriangle->Unlock();

    // create right triangle
    g_pd3dDevice->CreateVertexBuffer(
        3 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &g_pRightTriangle,
        0);

    Vertex* v2;
    g_pRightTriangle->Lock(0, 0, (void**)&v2, 0);

    v2[0] = Vertex(-1.0f,  1.0f, 0.0f, red);
    v2[1] = Vertex( 1.0f, -1.0f, 0.0f, red);
    v2[2] = Vertex(-1.0f, -1.0f, 0.0f, red);

    g_pRightTriangle->Unlock();

    return g_pLeftTriangle && g_pRightTriangle ? true : false;
}

void cleanup()
{
    SAFE_RELEASE(g_pLeftTriangle);
    SAFE_RELEASE(g_pRightTriangle);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

VOID render()
{
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

    D3DXMATRIX W;
    D3DXMatrixIdentity(&W);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &W);

    g_pd3dDevice->SetFVF(Vertex::FVF);

    g_pd3dDevice->BeginScene();
    {
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

        if(g_leftObjectTop) {
            drawRightTriangle();
            drawLeftTriangle();
        }
        else {
            drawLeftTriangle();
            drawRightTriangle();
        }

        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    }
    g_pd3dDevice->EndScene();
    
    camera();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void camera()
{
    FLOAT distance = -OBJECT_SIZE * 2;
    D3DXVECTOR3 position(0, 0, distance);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void drawLeftTriangle()
{
    D3DXMATRIX LeftTR;
    D3DXMatrixTranslation(&LeftTR, -0.25f, 0, 0);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &LeftTR);
    g_pd3dDevice->SetStreamSource(0, g_pLeftTriangle, 0, sizeof(Vertex));
    g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 3);
}

void drawRightTriangle()
{
    D3DXMATRIX LeftTR;
    D3DXMatrixTranslation(&LeftTR, 0.5f, 0, 0);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &LeftTR);
    g_pd3dDevice->SetStreamSource(0, g_pRightTriangle, 0, sizeof(Vertex));
    g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 3);
}

void resize(UINT width, UINT height)
{
    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
        &proj,
        D3DX_PI * 0.5f,
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
        
    default:
        g_leftObjectTop = !g_leftObjectTop;
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