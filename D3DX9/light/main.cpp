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
#include <d3dx9mesh.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

static const FLOAT OBJECT_SIZE = 10;
static const UINT OBJECT_PRECESSION = 100;

static const FLOAT ROTATION_DELTA = 0.05f;
static const FLOAT MAX_ANGLE = 360;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPD3DXMESH g_pObjectMesh = NULL;

D3DLIGHT9* g_pLight0 = NULL;
D3DLIGHT9* g_pLight1 = NULL;
D3DLIGHT9* g_pLight2 = NULL;
D3DLIGHT9* g_pLight3 = NULL;

static FLOAT g_rotation = 0;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
void setupLights();
void generateGeometry();
void cleanup();

void render();
void camera();
void setLightsPosition();

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

    HWND hWnd = CreateWindow(appClass, "D3D9 light",
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
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

    if(!g_pd3dDevice) {
        return false;
    }

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

    // init light
    setupLights();

    // init geometry
    generateGeometry();

    return true;
}

void setupLights()
{
    // config material
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Diffuse.r = material.Specular.r = material.Ambient.r = 1.0f;
    material.Diffuse.g = material.Specular.g = material.Ambient.g = 1.0f;
    material.Diffuse.b = material.Specular.b = material.Ambient.b = 1.0f;
    material.Diffuse.a = material.Specular.a = material.Ambient.a = 1.0f;
    g_pd3dDevice->SetMaterial(&material);

    // config light
    D3DXCOLOR whiteColor = D3DCOLOR_XRGB(255, 255, 255);
    D3DXCOLOR redColor = D3DCOLOR_XRGB(255, 0, 0);
    D3DXCOLOR greenColor = D3DCOLOR_XRGB(0, 255, 0);
    D3DXCOLOR blueColor = D3DCOLOR_XRGB(0, 0, 255);

    FLOAT range = 10.0f;

    g_pLight0 = new D3DLIGHT9;
    ZeroMemory(g_pLight0, sizeof(D3DLIGHT9));
    g_pLight0->Type = D3DLIGHT_DIRECTIONAL;
    g_pLight0->Ambient = whiteColor * 0.25f;
    D3DXVECTOR3 direction(1.0f, 1.0f, 1.0f);
    D3DXVec3Normalize((D3DXVECTOR3*)&(g_pLight0->Direction), &direction);

    g_pLight1 = new D3DLIGHT9;
    ZeroMemory(g_pLight1, sizeof(D3DLIGHT9));
    g_pLight1->Type = D3DLIGHT_POINT;
    g_pLight1->Diffuse = greenColor;
    g_pLight1->Specular = greenColor;
    g_pLight1->Range = range;

    g_pLight2 = new D3DLIGHT9;
    ZeroMemory(g_pLight2, sizeof(D3DLIGHT9));
    g_pLight2->Type = D3DLIGHT_POINT;
    g_pLight2->Diffuse = blueColor;
    g_pLight2->Specular = blueColor;
    g_pLight2->Range = range;

    g_pLight3 = new D3DLIGHT9;
    ZeroMemory(g_pLight3, sizeof(D3DLIGHT9));
    g_pLight3->Type = D3DLIGHT_POINT;
    g_pLight3->Diffuse = redColor;
    g_pLight3->Specular = redColor;
    g_pLight3->Range = range;

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    g_pd3dDevice->LightEnable(0, TRUE);
    g_pd3dDevice->LightEnable(1, TRUE);
    g_pd3dDevice->LightEnable(2, TRUE);
    g_pd3dDevice->LightEnable(3, TRUE);

    g_pd3dDevice->SetLight(0, g_pLight0);
}

void generateGeometry()
{
    D3DXCreateSphere(g_pd3dDevice, OBJECT_SIZE, OBJECT_PRECESSION, OBJECT_PRECESSION, &g_pObjectMesh, 0);
}

void cleanup()
{
    delete g_pLight0;
    delete g_pLight1;
    delete g_pLight2;
    delete g_pLight3;

    SAFE_RELEASE(g_pObjectMesh);
    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

VOID render()
{
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0);

    setLightsPosition();

    g_pd3dDevice->BeginScene();
    {
        g_pObjectMesh->DrawSubset(0);
    }
    g_pd3dDevice->EndScene();

    camera();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void camera()
{
    FLOAT distance = OBJECT_SIZE * 1.5f;
    D3DXVECTOR3 position(distance, distance, distance);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 0.0f, 1.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void setLightsPosition()
{
    FLOAT distance = OBJECT_SIZE * 1.5f;
    {
        FLOAT y = sin(g_rotation + D3DX_PI / 2) * distance;
        FLOAT z = cos(g_rotation + D3DX_PI / 2) * distance;
        D3DXVECTOR3 position = D3DXVECTOR3(0, y, z);
        g_pLight1->Position = position;
        g_pd3dDevice->SetLight(1, g_pLight1);
    }

    {
        FLOAT x = sin(g_rotation + D3DX_PI) * distance;
        FLOAT z = cos(g_rotation + D3DX_PI) * distance;
        D3DXVECTOR3 position = D3DXVECTOR3(x, 0, z);
        g_pLight2->Position = position;
        g_pd3dDevice->SetLight(2, g_pLight2);
    }

    {
        FLOAT x = sin(g_rotation) * distance;
        FLOAT y = cos(g_rotation) * distance;
        D3DXVECTOR3 position = D3DXVECTOR3(x, y, 0);
        g_pLight3->Position = position;
        g_pd3dDevice->SetLight(3, g_pLight3);
    }

    g_rotation += ROTATION_DELTA;
    if(g_rotation > MAX_ANGLE)
        g_rotation -= MAX_ANGLE;
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