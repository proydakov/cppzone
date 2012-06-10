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

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

struct Vertex
{
    Vertex(D3DXVECTOR3 point, D3DXVECTOR3 normal) :
        x(point.x), y(point.y), z(point.z),
        nx(normal.x), ny(normal.y), nz(normal.z)
        {
        }

    FLOAT x, y, z;
    FLOAT nx, ny, nz;
    static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

static const FLOAT FLOOR_SIZE = 30.0f;
static const FLOAT MAX_ROTATION = D3DX_PI * 2;
static const FLOAT ROTATION_TEAPOT_DELTA = 0.025f;
static const FLOAT ROTATION_LIGHT_DELTA = 0.015f;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

D3DLIGHT9* g_pPointLight = NULL;
D3DXVECTOR3 g_lightPosition = D3DXVECTOR3(FLOOR_SIZE / 2, FLOOR_SIZE * 2, FLOOR_SIZE / 2);

D3DMATERIAL9* g_pShadowMaterial = NULL;

LPD3DXMESH    g_pTeapotMesh     = NULL;
D3DMATERIAL9* g_pTeapotMaterial = NULL;
D3DXVECTOR3   g_teapotPosition  = D3DXVECTOR3(FLOOR_SIZE / 2, FLOOR_SIZE / 2, FLOOR_SIZE / 2);

LPDIRECT3DVERTEXBUFFER9 g_pFloor = NULL;
D3DMATERIAL9* g_pFloorMaterial   = NULL;
D3DXPLANE g_floorPlane = D3DXPLANE(0.0f, 1.0f, 0.0f, 0.0f);

D3DXVECTOR3 g_cameraPositionPoint = D3DXVECTOR3(FLOOR_SIZE, FLOOR_SIZE * 1.25f, 0);
D3DXVECTOR3 g_cameraTargetPoint = D3DXVECTOR3(FLOOR_SIZE / 2, 0, FLOOR_SIZE / 2);

FLOAT g_teapotRotate = 0;
FLOAT g_lightRotate = 0;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool initGeometry();
bool initMaterial();
bool initLight();
void cleanup();

void render();
void camera();
void moveTeapot();
void moveLight();

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

    HWND hWnd = CreateWindow(appClass, "D3D9 falt shadow",
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

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    g_pd3dDevice->LightEnable(0, TRUE);

    bool res = initGeometry();
    res &= initMaterial();
    res &= initLight();

    return res;
}

bool initGeometry()
{
    // init floor
    g_pd3dDevice->CreateVertexBuffer(
        6 * sizeof(Vertex),
        D3DUSAGE_WRITEONLY,
        Vertex::FVF,
        D3DPOOL_MANAGED,
        &g_pFloor,
        0);

    D3DXVECTOR3 points[8];
    points[0] = D3DXVECTOR3( 0.0f,       0.0f,       0.0f );
    points[1] = D3DXVECTOR3( FLOOR_SIZE, 0.0f,       0.0f );
    points[2] = D3DXVECTOR3( FLOOR_SIZE, 0.0f, FLOOR_SIZE );
    points[3] = D3DXVECTOR3( 0.0f,       0.0f, FLOOR_SIZE );

    D3DXVECTOR3 normal(0.0f, 1.0f, 0.0f);

    Vertex* v;
    g_pFloor->Lock(0, 0, (void**)&v, 0);

    v[0] = Vertex(points[1], normal);
    v[1] = Vertex(points[3], normal);
    v[2] = Vertex(points[0], normal);
    v[3] = Vertex(points[1], normal);
    v[4] = Vertex(points[2], normal);
    v[5] = Vertex(points[3], normal);

    g_pFloor->Unlock();

    if(!g_pFloor) {
        return false;
    }

    // init teapot
    D3DXCreateTeapot(g_pd3dDevice, &g_pTeapotMesh, 0);
    return g_pTeapotMesh ? true : false;
}

bool initMaterial()
{
    D3DMATERIAL9 blackMaterial;
    ZeroMemory(&blackMaterial, sizeof(D3DMATERIAL9));
    blackMaterial.Diffuse.r = blackMaterial.Specular.r = blackMaterial.Ambient.r = 0.0f;
    blackMaterial.Diffuse.g = blackMaterial.Specular.g = blackMaterial.Ambient.g = 0.0f;
    blackMaterial.Diffuse.b = blackMaterial.Specular.b = blackMaterial.Ambient.b = 0.0f;
    blackMaterial.Diffuse.a = blackMaterial.Specular.a = blackMaterial.Ambient.a = 0.5f;

    D3DMATERIAL9 blueMaterial;
    ZeroMemory(&blueMaterial, sizeof(D3DMATERIAL9));
    blueMaterial.Diffuse.r = blueMaterial.Specular.r = blueMaterial.Ambient.r = 0.0f;
    blueMaterial.Diffuse.g = blueMaterial.Specular.g = blueMaterial.Ambient.g = 0.0f;
    blueMaterial.Diffuse.b = blueMaterial.Specular.b = blueMaterial.Ambient.b = 1.0f;
    blueMaterial.Diffuse.a = blueMaterial.Specular.a = blueMaterial.Ambient.a = 1.0f;
    
    D3DMATERIAL9 grayMaterial;
    ZeroMemory(&grayMaterial, sizeof(D3DMATERIAL9));
    grayMaterial.Diffuse.r = grayMaterial.Specular.r = grayMaterial.Ambient.r = 0.2f;
    grayMaterial.Diffuse.g = grayMaterial.Specular.g = grayMaterial.Ambient.g = 0.2f;
    grayMaterial.Diffuse.b = grayMaterial.Specular.b = grayMaterial.Ambient.b = 0.2f;
    grayMaterial.Diffuse.a = grayMaterial.Specular.a = grayMaterial.Ambient.a = 1.0f;

    g_pShadowMaterial = new D3DMATERIAL9(blackMaterial);
    g_pTeapotMaterial = new D3DMATERIAL9(blueMaterial);
    g_pFloorMaterial = new D3DMATERIAL9(grayMaterial);

    if(!g_pShadowMaterial || !g_pTeapotMaterial || !g_pFloorMaterial) {
        return false;
    }
    return true;
}

bool initLight()
{
    D3DXCOLOR whiteColor = D3DCOLOR_XRGB(255, 255, 255);

    FLOAT range = 120.0f;

    g_pPointLight = new D3DLIGHT9;
    ZeroMemory(g_pPointLight, sizeof(D3DLIGHT9));
    g_pPointLight->Type = D3DLIGHT_POINT;
    g_pPointLight->Diffuse = whiteColor * 0.5f;
    g_pPointLight->Specular = whiteColor * 0.75f;
    g_pPointLight->Range = range;

    if(!g_pPointLight) {
        return false;
    }
    return true;
}

void cleanup()
{
    delete g_pPointLight;
    delete g_pShadowMaterial;
    delete g_pTeapotMaterial;
    delete g_pFloorMaterial;

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

        // setup light
        D3DXMatrixIdentity(&MWORLD);
        g_pPointLight->Position = g_lightPosition;
        g_pd3dDevice->SetLight(0, g_pPointLight);

        // draw floor
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);
        g_pd3dDevice->SetMaterial(g_pFloorMaterial);

        g_pd3dDevice->SetStreamSource(0, g_pFloor, 0, sizeof(Vertex));
        g_pd3dDevice->SetFVF(Vertex::FVF);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 6);
    
        // draw shadow
        D3DXMATRIX MTRANS;
        D3DXMatrixTranslation(&MTRANS, g_teapotPosition.x, g_teapotPosition.y, g_teapotPosition.z);
        D3DXMATRIX MROTATE;
        D3DXMatrixRotationY(&MROTATE, g_teapotRotate);
        MWORLD = MROTATE * MTRANS;
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);
        g_pd3dDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

        g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

        D3DXVECTOR4 lightPosition(g_lightPosition.x, g_lightPosition.y, g_lightPosition.z, 1);

        D3DXMATRIX MSHADOW;
        D3DXMatrixShadow(&MSHADOW, &lightPosition, &g_floorPlane);
        MWORLD = MROTATE * MTRANS * MSHADOW;
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);

        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetMaterial(g_pShadowMaterial);
        g_pTeapotMesh->DrawSubset(0);
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

        // draw teapot
        MWORLD = MROTATE * MTRANS;
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MWORLD);

        g_pd3dDevice->SetMaterial(g_pTeapotMaterial);
        g_pTeapotMesh->DrawSubset(0);
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
    moveTeapot();
    moveLight();
}

void camera()
{
    D3DXVECTOR3 position(g_cameraPositionPoint.x, g_cameraPositionPoint.y, g_cameraPositionPoint.z);
    D3DXVECTOR3 target(g_cameraTargetPoint.x, g_cameraTargetPoint.y, g_cameraTargetPoint.z);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX V;
    D3DXMatrixLookAtLH(&V, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void moveTeapot()
{
    // rotate
    g_teapotRotate += ROTATION_TEAPOT_DELTA;
    if(g_teapotRotate > MAX_ROTATION) {
        g_teapotRotate -= MAX_ROTATION;
    }
}

void moveLight()
{
    // rotate
    g_lightRotate += ROTATION_LIGHT_DELTA;
    if(g_lightRotate > MAX_ROTATION) {
        g_lightRotate -= MAX_ROTATION;
    }
    g_lightPosition.x = FLOOR_SIZE / 2 + sin(g_lightRotate) * FLOOR_SIZE * 1.1f;
    g_lightPosition.z = FLOOR_SIZE / 2 + cos(g_lightRotate) * FLOOR_SIZE * 1.1f;
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
        resize(rect.right - rect.left, rect.bottom - rect.top);
        break;

    case WM_KEYDOWN:
        keyboard(hWnd, wParam);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}