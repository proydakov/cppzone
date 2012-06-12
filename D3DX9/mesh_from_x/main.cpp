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

#include <vector>
#include <string>
#include <cassert>

#include <d3d9.h>
#include <d3dx9mesh.h>

#include <config_directx_mesh_from_x.h>

#pragma warning (disable:4996)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

static const FLOAT ROTATE_DELTA = 0.01f;
static const FLOAT ROTATE_MAX = D3DX_PI * 2;

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

LPD3DXMESH g_pMeshObject = NULL;
std::vector<D3DMATERIAL9> g_objectMaterials;
std::vector<LPDIRECT3DTEXTURE9> g_objectTextures;

FLOAT g_rotate = 0;

std::string g_app_name;

//-----------------------------------------------------------------------------
// Forward declarations 
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool initModel();
void setTextureFilter();
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

    g_app_name = std::string("D3D9 mesh from X");

    HWND hWnd = CreateWindow(appClass, g_app_name.c_str(),
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
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;

    g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);

    if(!g_pd3dDevice) {
        return false;
    }

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    bool res = initModel();

    return res;
}

bool initModel()
{
    std::string model_directory(MODELS_DIRECTORY);
    std::string model(model_directory + "dwarf.x");

    LPD3DXBUFFER pD3DXMtrlBuffer;

    DWORD numMtrls = 0;
    D3DXLoadMeshFromX(
        model.c_str(),
        D3DXMESH_MANAGED,
        g_pd3dDevice,
        NULL,
        &pD3DXMtrlBuffer,
        NULL,
        &numMtrls,
        &g_pMeshObject);

    if(!g_pMeshObject) {
        return false;
    }

    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

    g_objectTextures.resize(numMtrls);

    for(size_t i = 0; i < numMtrls; ++i) {
        g_objectMaterials.push_back(d3dxMaterials[i].MatD3D);

        g_objectMaterials[i].Ambient = g_objectMaterials[i].Specular = g_objectMaterials[i].Diffuse;

        std::string texture_name(d3dxMaterials[i].pTextureFilename);
        std::string texture(model_directory + texture_name);

        if(d3dxMaterials[i].pTextureFilename != NULL && texture.size()) {
            D3DXCreateTextureFromFile(
                g_pd3dDevice, 
                texture.c_str(), 
                &g_objectTextures[i]);

            if(!g_objectTextures[i]) {
                std::string message("Could not find texture map ");
                message += texture;
                MessageBox(NULL, message.c_str(), g_app_name.c_str(), MB_OK);
            }
            else {
                setTextureFilter();
            }
        }
    }
    pD3DXMtrlBuffer->Release();

    return true;
}

void setTextureFilter()
{
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
}

void cleanup()
{
    for(size_t i = 0; i < g_objectTextures.size(); ++i) {
        SAFE_RELEASE(g_objectTextures[i]);
    }

    SAFE_RELEASE(g_pMeshObject);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_pd3dDevice->BeginScene();
    {
        D3DXMATRIX MROTATE;
        D3DXMatrixRotationY(&MROTATE, g_rotate);
        g_pd3dDevice->SetTransform(D3DTS_WORLD, &MROTATE);

        g_pd3dDevice->BeginScene();
        for(size_t i = 0; i < g_objectMaterials.size(); i++)
        {
            g_pd3dDevice->SetMaterial(&g_objectMaterials[i]);
            g_pd3dDevice->SetTexture(0, g_objectTextures[i]);
            g_pMeshObject->DrawSubset(i);
        }
        g_pd3dDevice->EndScene();
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
    rotate();
}

void rotate()
{
    g_rotate += ROTATE_DELTA;
    if(g_rotate > ROTATE_MAX) {
        g_rotate -= ROTATE_MAX;
    }
}

void camera()
{
    D3DXVECTOR3 position(0.0f, 2.0f, -3.0f);
    D3DXVECTOR3 target(0.0f, 1.0f, 0.0f);
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