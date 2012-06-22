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

#include <geometry_loader.h>
#include <mesh_generator.h>

#include <config_terrain_height_map.h>

#pragma warning (disable:4996)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS g_pd3dPresentParam;

//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pDirtTexture  = NULL;
LPDIRECT3DTEXTURE9 g_pStoneTexture = NULL;
LPDIRECT3DTEXTURE9 g_pGrassTexture = NULL;
LPDIRECT3DTEXTURE9 g_pBlendMapTexture = NULL;

//-----------------------------------------------------------------------------
LPD3DXMESH g_pTerrianMesh = NULL;
FLOAT g_side_size     = 1.0f;
FLOAT g_horizontal_scale = 1.0f;
FLOAT g_vertical_scale   = 1.0f;

//-----------------------------------------------------------------------------
LPD3DXEFFECT g_pEffect = NULL;
D3DXHANDLE g_hTech;
D3DXHANDLE g_hViewProj;
D3DXHANDLE g_hDirToSunW;
D3DXHANDLE g_hGrassTex;
D3DXHANDLE g_hDirtTex;
D3DXHANDLE g_hStoneTex;
D3DXHANDLE g_hBlendMap;

//-----------------------------------------------------------------------------
terrain::geometry_loader* g_pTerrainGeometryLoader = NULL;
terrain::mesh_generator*  g_pTerrainMeshGenerator  = NULL;

//-----------------------------------------------------------------------------
D3DXMATRIX g_viewMatrix;
D3DXMATRIX g_projMatrix;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool init_present_param();
bool init_terrain();
bool init_textures();
bool init_effect();
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

    HWND hWnd = CreateWindow(appClass, "D3D9 terrain height map",
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
    if(!init_present_param()) {
        return false;
    }
    g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_pd3dPresentParam, &g_pd3dDevice);

    if(!g_pd3dDevice) {
        return false;
    }

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    g_pTerrainGeometryLoader = new terrain::geometry_loader;
    g_pTerrainMeshGenerator = new terrain::mesh_generator;

    bool res = true;
    res &= init_terrain();
    res &= init_textures();
    res &= init_effect();

    return res;
}

bool init_present_param()
{
    ZeroMemory(&g_pd3dPresentParam, sizeof(g_pd3dPresentParam));
    g_pd3dPresentParam.Windowed = TRUE;
    g_pd3dPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_pd3dPresentParam.BackBufferFormat = D3DFMT_UNKNOWN;
    g_pd3dPresentParam.EnableAutoDepthStencil = TRUE;
    g_pd3dPresentParam.AutoDepthStencilFormat = D3DFMT_D24X8;

    return true;
}

bool init_terrain()
{
    std::string raw_directory(TERRAIN_DIRECTORY);
    std::string raw_file_name(raw_directory + "terrain.raw");

    unsigned map_side_size = 256;
    FLOAT h_scale = 10;
    FLOAT v_scale = 4;

    g_side_size = static_cast<FLOAT>(map_side_size);
    g_horizontal_scale = h_scale;
    g_vertical_scale = v_scale;

    bool res = true;
    res &= g_pTerrainGeometryLoader->load_raw(raw_file_name, map_side_size, map_side_size);
    res &= g_pTerrainMeshGenerator->generate_mesh(g_pd3dDevice, g_pTerrainGeometryLoader, h_scale, v_scale, &g_pTerrianMesh);

    return res;
}

bool init_textures()
{
    std::string parent_dir(TEXTURES_DIRECTORY);
    std::string dirt_texture (parent_dir + "dirt.dds");
    std::string grass_texture(parent_dir + "grass.dds");
    std::string stone_texture(parent_dir + "stone.dds");
    std::string blend_texture(parent_dir + "blend_map.dds");

    D3DXCreateTextureFromFile(g_pd3dDevice, dirt_texture.c_str(),  &g_pDirtTexture );
    D3DXCreateTextureFromFile(g_pd3dDevice, grass_texture.c_str(), &g_pGrassTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, stone_texture.c_str(), &g_pStoneTexture);
    D3DXCreateTextureFromFile(g_pd3dDevice, blend_texture.c_str(), &g_pBlendMapTexture);

    if(!g_pDirtTexture || !g_pGrassTexture || !g_pStoneTexture || !g_pBlendMapTexture) {
        return false;
    }
    return true;
}

bool init_effect()
{
    std::string parent_dir(EFFECT_DIRECTORY);
    std::string effect(parent_dir + "terrain.fx");

    ID3DXBuffer* errors = 0;
    D3DXCreateEffectFromFile(g_pd3dDevice, effect.c_str(),
        0, 0, D3DXSHADER_DEBUG, 0, &g_pEffect, &errors);

    if(!g_pEffect) {
        return false;
    }

    if(errors) {
        MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(errors);
    }

    g_hTech      = g_pEffect->GetTechniqueByName("TerrainTech");
    g_hViewProj  = g_pEffect->GetParameterByName(0, "ViewProj");
    g_hDirToSunW = g_pEffect->GetParameterByName(0, "DirToSunW");
    g_hGrassTex  = g_pEffect->GetParameterByName(0, "GrassTex");
    g_hDirtTex   = g_pEffect->GetParameterByName(0, "DirtTex");
    g_hStoneTex  = g_pEffect->GetParameterByName(0, "StoneTex");
    g_hBlendMap  = g_pEffect->GetParameterByName(0, "BlendMap");

    D3DXVECTOR4 dirToSun(0.0f, 1.0f, 0.0f, 0.0f);

    g_pEffect->SetTexture(g_hGrassTex, g_pGrassTexture);
    g_pEffect->SetTexture(g_hDirtTex, g_pDirtTexture);
    g_pEffect->SetTexture(g_hStoneTex, g_pStoneTexture);
    g_pEffect->SetTexture(g_hBlendMap, g_pBlendMapTexture);
    g_pEffect->SetVector(g_hDirToSunW, &dirToSun);

    return true;
}

void cleanup()
{
    delete g_pTerrainGeometryLoader;
    delete g_pTerrainMeshGenerator;

    SAFE_RELEASE(g_pDirtTexture);
    SAFE_RELEASE(g_pStoneTexture);
    SAFE_RELEASE(g_pGrassTexture);
    SAFE_RELEASE(g_pBlendMapTexture);

    SAFE_RELEASE(g_pEffect);
    SAFE_RELEASE(g_pTerrianMesh);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    g_pd3dDevice->BeginScene();
    {
        //g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        D3DXMATRIX ViewProj = g_viewMatrix * g_projMatrix;
        g_pEffect->SetMatrix(g_hViewProj, &ViewProj);
        g_pEffect->SetTechnique(g_hTech);

        UINT numPasses = 0;
        g_pEffect->Begin(&numPasses, 0);
        g_pEffect->BeginPass(0);

        g_pTerrianMesh->DrawSubset(0);

        g_pEffect->EndPass();
        g_pEffect->End();
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
}

void camera()
{
    FLOAT h = g_side_size * g_horizontal_scale;
    FLOAT v = g_side_size * g_horizontal_scale;

    D3DXVECTOR3 position(-h / 10, v, -h / 10);
    D3DXVECTOR3 target(h * 3 / 5, 0.0f, h * 3 / 5);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX view;
    D3DXMatrixLookAtLH(&view, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &view);

    g_viewMatrix = view;
}

void resize(UINT width, UINT height)
{
    g_pd3dPresentParam.BackBufferWidth = width;
    g_pd3dPresentParam.BackBufferHeight = height;
    g_pEffect->OnLostDevice();
    g_pd3dDevice->Reset(&g_pd3dPresentParam);
    g_pEffect->OnResetDevice();

    D3DXMATRIX proj;
    D3DXMatrixPerspectiveFovLH(
        &proj,
        D3DX_PI * 0.25f,
        (FLOAT) width / (FLOAT) height,
        0.1f,
        10000.0f);
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

#pragma warning (default:4996)