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

#include <ctime>
#include <string>
#include <cassert>

#include <d3d9.h>
#include <d3dx9mesh.h>

#pragma warning (disable:4996)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPRECT g_pRect = NULL;
LPD3DXFONT g_pFont = NULL;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool initFont();
void initSystem();
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

    HWND hWnd = CreateWindow(appClass, "D3D9 font",
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

    bool res = initFont();
    initSystem();

    return res;
}

bool initFont()
{
    D3DXFONT_DESC lf;
    ZeroMemory(&lf, sizeof(D3DXFONT_DESC));
    lf.Height = 30;
    lf.Width = 12;
    lf.Weight = 500;
    lf.Italic = false;
    lf.CharSet = DEFAULT_CHARSET;
    strcpy(lf.FaceName, "Times New Roman");

    D3DXCreateFontIndirect(g_pd3dDevice, &lf, &g_pFont);
    
    g_pRect = new RECT;

    return g_pFont ? true : false;
}

void initSystem()
{
    srand((unsigned int)time(NULL));
}

void cleanup()
{
    delete g_pRect;
    SAFE_RELEASE(g_pFont);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    D3DCOLOR color[3];
    color[0] = D3DCOLOR_XRGB(255, 0, 0);
    color[1] = D3DCOLOR_XRGB(0, 255, 0);
    color[2] = D3DCOLOR_XRGB(0, 0, 255);

    int i = 0;
    int step = 50;

    int delta = 0;
    int max_delta = 2;

    g_pd3dDevice->BeginScene();
    {
        RECT rect(*g_pRect);
        while(rect.top < rect.bottom) {
            delta = (rand() % max_delta) - max_delta / 2;
            rect.top += delta;
            g_pFont->DrawText(NULL, "This text is created using ID3DXFont =)))", -1, &rect, DT_TOP | DT_LEFT, color[i]);
            i = ++i % 3;
            rect.top += step;
        }
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
}

void camera()
{
    D3DXVECTOR3 position(0.0f, 0.0f, -1.0f);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
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
        //ValidateRect(hWnd, NULL);
        return 0;

    case WM_SIZE:
        RECT rect;
        GetClientRect(hWnd, &rect);
        memcpy(g_pRect, &rect, sizeof(RECT));
        resize(rect.right - rect.left, rect.bottom - rect.top);
        break;

    case WM_KEYDOWN:
        keyboard(hWnd, wParam);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma warning (default:4996)