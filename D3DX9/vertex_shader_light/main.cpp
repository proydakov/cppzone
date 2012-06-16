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

#include <config_directx_vertex_shader_light.h>

#pragma warning (disable:4996)

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = NULL; } }
#endif

#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------
LPDIRECT3D9       g_pD3D       = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

//-----------------------------------------------------------------------------
LPD3DXFONT g_pFont = NULL;

//-----------------------------------------------------------------------------
LPD3DXMESH g_pTeapotMesh = NULL;

//-----------------------------------------------------------------------------
D3DLIGHT9* g_pLight = NULL;
D3DMATERIAL9* g_pMaterial = NULL;

//-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE g_pAmbientShaderConstantTable = NULL;
LPDIRECT3DVERTEXSHADER9 g_pAmbientShader          = NULL;
D3DXHANDLE g_ambientShaderViewProjMatrixHandle = 0;
D3DXHANDLE g_ambientShaderMtrlHandle           = 0;
D3DXHANDLE g_ambientLightIntensityHandle       = 0;

//-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE g_pDiffuseShaderConstantTable = NULL;
LPDIRECT3DVERTEXSHADER9 g_pDiffuseShader          = NULL;
D3DXHANDLE g_diffuseShaderViewWorldMatrixHandle = 0;
D3DXHANDLE g_diffuseShaderViewProjMatrixHandle  = 0;
D3DXHANDLE g_diffuseShaderLightDirHandle        = 0;
D3DXHANDLE g_diffuseShaderMtrlHandle            = 0;
D3DXHANDLE g_diffuseShaderLightIntHandle        = 0;

//-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE g_pSpecularShaderConstantTable = NULL;
LPDIRECT3DVERTEXSHADER9 g_pSpecularShader          = NULL;
D3DXHANDLE g_specularShaderViewWorldMatrixHandle = 0;
D3DXHANDLE g_specularShaderViewProjMatrixHandle  = 0;
D3DXHANDLE g_specularShaderLightDirHandle        = 0;
D3DXHANDLE g_specularShaderViewDirHandle         = 0;
D3DXHANDLE g_specularShaderMtrlHandle            = 0;
D3DXHANDLE g_specularShaderLightIntHandle        = 0;

//-----------------------------------------------------------------------------
LPD3DXCONSTANTTABLE g_pComboShaderConstantTable = NULL;
LPDIRECT3DVERTEXSHADER9 g_pComboShader          = NULL;
D3DXHANDLE g_comboShaderViewWorldMatrixHandle  = 0;
D3DXHANDLE g_comboShaderViewProjMatrixHandle   = 0;
D3DXHANDLE g_comboShaderLightDirHandle         = 0;
D3DXHANDLE g_comboShaderViewDirHandle          = 0;
D3DXHANDLE g_comboShaderAmbientMtrlHandle      = 0;
D3DXHANDLE g_comboShaderDiffuseMtrlHandle      = 0;
D3DXHANDLE g_comboShaderSpecularMtrlHandle     = 0;
D3DXHANDLE g_comboShaderAmbientLightIntHandle  = 0;
D3DXHANDLE g_comboShaderDiffuseLightIntHandle  = 0;
D3DXHANDLE g_comboShaderSpecularLightIntHandle = 0;

//-----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 g_pCurrentShader = NULL;
LPDIRECT3DVERTEXSHADER9 g_pLastCurrentShader = NULL;

//-----------------------------------------------------------------------------
D3DXMATRIX g_projMatrix;
D3DXMATRIX g_viewMatrix;
D3DXVECTOR4 g_viewVector;

//-----------------------------------------------------------------------------
static const FLOAT MAX_ROTATE = D3DX_PI * 2;
static const FLOAT ROTATE_DELTA = 0.017f;
FLOAT g_rotate = 0.0f;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
bool init(HWND hWnd);
bool initFont();
bool init_object();
bool init_light();
bool init_material();
bool init_shaders();
bool init_ambient_shader(const std::string& shader_dir);
bool init_diffuse_shader(const std::string& shader_dir);
bool init_specular_shader(const std::string& shader_dir);
bool init_combo_shader(const std::string& shader_dir);
void cleanup();

void render();
void config_ambient_shader();
void config_diffuse_shader();
void config_specular_shader();
void config_combo_shader();
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

    HWND hWnd = CreateWindow(appClass, "D3D9 vertex shader light",
        WS_OVERLAPPEDWINDOW, 100, 100, 640, 640,
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
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    bool res = true;
    res &= initFont();
    res &= init_object();
    res &= init_light();
    res &= init_material();
    res &= init_shaders();

    return res;
}

bool initFont()
{
    D3DXFONT_DESC lf;
    ZeroMemory(&lf, sizeof(D3DXFONT_DESC));
    lf.Height = 22;
    lf.Width = 8;
    lf.Weight = 500;
    lf.Italic = false;
    lf.CharSet = DEFAULT_CHARSET;
    strcpy(lf.FaceName, "Times New Roman");

    D3DXCreateFontIndirect(g_pd3dDevice, &lf, &g_pFont);

    return g_pFont ? true : false;
}

bool init_object()
{
    D3DXCreateTeapot(g_pd3dDevice, &g_pTeapotMesh, 0);

    return g_pTeapotMesh ? true : false;
}

bool init_light()
{
    D3DXCOLOR lightColor = D3DCOLOR_XRGB(255, 0, 0);

    g_pLight = new D3DLIGHT9;
    ZeroMemory(g_pLight, sizeof(D3DLIGHT9));
    D3DXVECTOR3 direction(0.0f, 2.0f, -2.0f);
    D3DXVec3Normalize((D3DXVECTOR3*)&(g_pLight->Direction), &direction);
    g_pLight->Ambient = lightColor * 0.15f;
    g_pLight->Diffuse = lightColor * 0.35f;
    g_pLight->Specular = lightColor * 0.75f;

    return g_pLight ? true : false;
}

bool init_material()
{
    D3DXCOLOR whiteColor = D3DCOLOR_XRGB(255, 255, 255);

    g_pMaterial = new D3DMATERIAL9;
    ZeroMemory(g_pMaterial, sizeof(D3DMATERIAL9));
    g_pMaterial->Ambient = whiteColor;
    g_pMaterial->Diffuse = whiteColor;
    g_pMaterial->Specular = whiteColor;

    return g_pMaterial ? true : false;
}

bool init_shaders()
{
    std::string base_directory(SHADER_DIRECTORY);

    bool res = true;
    res &= init_ambient_shader(base_directory);
    res &= init_diffuse_shader(base_directory);
    res &= init_specular_shader(base_directory);
    res &= init_combo_shader(base_directory);

    g_pCurrentShader = g_pComboShader;

    return true;
}

bool init_ambient_shader(const std::string& shader_dir)
{
    std::string shader_name("ambient.hlsl");
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
        &g_pAmbientShaderConstantTable);

    if(pErrorBuffer) {
        MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(pErrorBuffer);
    }
    if(!pShader) {
        return false;
    }

    g_pd3dDevice->CreateVertexShader(
        (DWORD*)pShader->GetBufferPointer(),
        &g_pAmbientShader);

    SAFE_RELEASE(pShader);

    if(!g_pAmbientShader) {
        return false;
    }

    g_ambientShaderViewProjMatrixHandle = g_pAmbientShaderConstantTable->GetConstantByName(0, "ViewProjMatrix");
    g_ambientShaderMtrlHandle = g_pAmbientShaderConstantTable->GetConstantByName(0, "AmbientMtrl");
    g_ambientLightIntensityHandle = g_pAmbientShaderConstantTable->GetConstantByName(0, "AmbientLightInt");

    return true;
}

bool init_diffuse_shader(const std::string& shader_dir)
{
    std::string shader_name("diffuse.hlsl");
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
        &g_pDiffuseShaderConstantTable);

    if(pErrorBuffer) {
        MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(pErrorBuffer);
    }
    if(!pShader) {
        return false;
    }

    g_pd3dDevice->CreateVertexShader(
        (DWORD*)pShader->GetBufferPointer(),
        &g_pDiffuseShader);

    SAFE_RELEASE(pShader);

    if(!g_pDiffuseShader) {
        return false;
    }

    g_diffuseShaderViewWorldMatrixHandle = g_pDiffuseShaderConstantTable->GetConstantByName(0, "ViewWorldMatrix");
    g_diffuseShaderViewProjMatrixHandle = g_pDiffuseShaderConstantTable->GetConstantByName(0, "ViewProjMatrix");
    g_diffuseShaderMtrlHandle = g_pDiffuseShaderConstantTable->GetConstantByName(0, "DiffuseMtrl");
    g_diffuseShaderLightDirHandle = g_pDiffuseShaderConstantTable->GetConstantByName(0, "LightDirection");
    g_diffuseShaderLightIntHandle = g_pDiffuseShaderConstantTable->GetConstantByName(0, "DiffuseLightInt");

    return true;
}

bool init_specular_shader(const std::string& shader_dir)
{
    std::string shader_name("specular.hlsl");
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
        &g_pSpecularShaderConstantTable);

    if(pErrorBuffer) {
        MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(pErrorBuffer);
    }
    if(!pShader) {
        return false;
    }

    g_pd3dDevice->CreateVertexShader(
        (DWORD*)pShader->GetBufferPointer(),
        &g_pSpecularShader);

    SAFE_RELEASE(pShader);

    if(!g_pSpecularShader) {
        return false;
    }

    g_specularShaderViewWorldMatrixHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "ViewWorldMatrix");
    g_specularShaderViewProjMatrixHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "ViewProjMatrix");
    g_specularShaderLightDirHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "LightDirection");
    g_specularShaderViewDirHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "ViewDirection");
    g_specularShaderMtrlHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "SpecularMtrl");
    g_specularShaderLightIntHandle = g_pSpecularShaderConstantTable->GetConstantByName(0, "SpecularLightInt");

    return true;
}

bool init_combo_shader(const std::string& shader_dir)
{
    std::string shader_name("combo.hlsl");
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
        &g_pComboShaderConstantTable);

    if(pErrorBuffer) {
        MessageBox(0, (char*)pErrorBuffer->GetBufferPointer(), 0, 0);
        SAFE_RELEASE(pErrorBuffer);
    }
    if(!pShader) {
        return false;
    }

    g_pd3dDevice->CreateVertexShader(
        (DWORD*)pShader->GetBufferPointer(),
        &g_pComboShader);

    SAFE_RELEASE(pShader);

    if(!g_pComboShader) {
        return false;
    }

    g_comboShaderViewWorldMatrixHandle = g_pComboShaderConstantTable->GetConstantByName(0, "ViewWorldMatrix");
    g_comboShaderViewProjMatrixHandle = g_pComboShaderConstantTable->GetConstantByName(0, "ViewProjMatrix");
    g_comboShaderLightDirHandle = g_pComboShaderConstantTable->GetConstantByName(0, "LightDirection");
    g_comboShaderViewDirHandle = g_pComboShaderConstantTable->GetConstantByName(0, "ViewDirection");
    g_comboShaderAmbientMtrlHandle = g_pComboShaderConstantTable->GetConstantByName(0, "AmbientMtrl");
    g_comboShaderDiffuseMtrlHandle = g_pComboShaderConstantTable->GetConstantByName(0, "DiffuseMtrl");
    g_comboShaderSpecularMtrlHandle = g_pComboShaderConstantTable->GetConstantByName(0, "SpecularMtrl");
    g_comboShaderAmbientLightIntHandle = g_pComboShaderConstantTable->GetConstantByName(0, "AmbientLightInt");
    g_comboShaderDiffuseLightIntHandle = g_pComboShaderConstantTable->GetConstantByName(0, "DiffuseLightInt");
    g_comboShaderSpecularLightIntHandle = g_pComboShaderConstantTable->GetConstantByName(0, "SpecularLightInt");

    return true;
}

void cleanup()
{
    delete g_pLight;
    delete g_pMaterial;

    SAFE_RELEASE(g_pFont);
    SAFE_RELEASE(g_pTeapotMesh);

    SAFE_RELEASE(g_pAmbientShaderConstantTable);
    SAFE_RELEASE(g_pAmbientShader);
    SAFE_RELEASE(g_pDiffuseShaderConstantTable);
    SAFE_RELEASE(g_pDiffuseShader);
    SAFE_RELEASE(g_pSpecularShaderConstantTable);
    SAFE_RELEASE(g_pSpecularShader);
    SAFE_RELEASE(g_pComboShaderConstantTable);
    SAFE_RELEASE(g_pComboShader);

    SAFE_RELEASE(g_pd3dDevice);
    SAFE_RELEASE(g_pD3D);
}

void render()
{
    assert(g_pd3dDevice);

    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    g_pd3dDevice->BeginScene();
    {
        {
            std::string text("\n\
                             - Press 1 to select ambient light model.\n\
                             - Press 2 to select diffuse light model.\n\
                             - Press 3 to select specular light model.\n\
                             - Press 4 to select combo light model.\n");

            RECT rect = {0, 0, 500, 150};
            g_pFont->DrawText(NULL, text.c_str(), -1, &rect, DT_TOP | DT_LEFT, D3DCOLOR_XRGB(0, 0, 255));
        }

        D3DXMATRIX MWORLD;
        D3DXMATRIX MWORLDVIEWINVERS;
        D3DXMatrixRotationY(&MWORLD, g_rotate);

        D3DXMATRIX ViewWorld = MWORLD * g_viewMatrix;
        D3DXMATRIX ViewProj = ViewWorld * g_projMatrix;

        D3DXVECTOR4 baseDirection(g_pLight->Direction.x, g_pLight->Direction.y, g_pLight->Direction.z, 0.0f);
        D3DXVECTOR4 directionToLight;
        D3DXVECTOR4 viewVector;
        D3DXMatrixTranspose(&MWORLD, &MWORLD);
        D3DXVec4Transform(&directionToLight, &baseDirection, &MWORLD);
        D3DXVec4Transform(&viewVector, &g_viewVector, &MWORLD);

        if(g_pCurrentShader == g_pAmbientShader) {
            g_pAmbientShaderConstantTable->SetMatrix(g_pd3dDevice, g_ambientShaderViewProjMatrixHandle, &ViewProj);
        }
        else if(g_pCurrentShader == g_pDiffuseShader) {
            g_pDiffuseShaderConstantTable->SetVector(g_pd3dDevice, g_diffuseShaderLightDirHandle, &directionToLight);
            g_pDiffuseShaderConstantTable->SetMatrix(g_pd3dDevice, g_diffuseShaderViewWorldMatrixHandle, &ViewWorld);
            g_pDiffuseShaderConstantTable->SetMatrix(g_pd3dDevice, g_diffuseShaderViewProjMatrixHandle, &ViewProj);
        }
        else if(g_pCurrentShader == g_pSpecularShader) {
            g_pSpecularShaderConstantTable->SetVector(g_pd3dDevice, g_specularShaderLightDirHandle, &directionToLight);
            g_pSpecularShaderConstantTable->SetVector(g_pd3dDevice, g_specularShaderViewDirHandle, &viewVector);
            g_pSpecularShaderConstantTable->SetMatrix(g_pd3dDevice, g_specularShaderViewWorldMatrixHandle, &ViewWorld);
            g_pSpecularShaderConstantTable->SetMatrix(g_pd3dDevice, g_specularShaderViewProjMatrixHandle, &ViewProj);
        }
        else if(g_pCurrentShader == g_pComboShader) {
            g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderLightDirHandle, &directionToLight);
            g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderViewDirHandle, &viewVector);
            g_pComboShaderConstantTable->SetMatrix(g_pd3dDevice, g_comboShaderViewWorldMatrixHandle, &ViewWorld);
            g_pComboShaderConstantTable->SetMatrix(g_pd3dDevice, g_comboShaderViewProjMatrixHandle, &ViewProj);
        }

        if(g_pCurrentShader != g_pLastCurrentShader) {
            if(g_pCurrentShader == g_pAmbientShader) {
                config_ambient_shader();
            }
            else if(g_pCurrentShader == g_pDiffuseShader) {
                config_diffuse_shader();
            }
            else if(g_pCurrentShader == g_pSpecularShader) {
                config_specular_shader();
            }
            else if(g_pCurrentShader == g_pComboShader) {
                config_combo_shader();
            }
            g_pLastCurrentShader = g_pCurrentShader;
        }

        g_pd3dDevice->SetVertexShader(g_pCurrentShader);
        g_pTeapotMesh->DrawSubset(0);
    }
    g_pd3dDevice->EndScene();

    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

    camera();
    rotate();
}

void config_ambient_shader()
{
    D3DXVECTOR4 ambientMtrl(g_pMaterial->Ambient.r, g_pMaterial->Ambient.g, g_pMaterial->Ambient.b, g_pMaterial->Ambient.a);
    D3DXVECTOR4 ambientLightInt(g_pLight->Ambient.r, g_pLight->Ambient.g, g_pLight->Ambient.b, g_pLight->Ambient.a);

    g_pAmbientShaderConstantTable->SetVector(g_pd3dDevice, g_ambientShaderMtrlHandle, &ambientMtrl);
    g_pAmbientShaderConstantTable->SetVector(g_pd3dDevice, g_ambientLightIntensityHandle, &ambientLightInt);
    g_pAmbientShaderConstantTable->SetDefaults(g_pd3dDevice);
}

void config_diffuse_shader()
{
    D3DXVECTOR4 diffuseMtrl(g_pMaterial->Diffuse.r, g_pMaterial->Diffuse.g, g_pMaterial->Diffuse.b, g_pMaterial->Diffuse.a);
    D3DXVECTOR4 diffuseLightInt(g_pLight->Diffuse.r, g_pLight->Diffuse.g, g_pLight->Diffuse.b, g_pLight->Diffuse.a);

    g_pDiffuseShaderConstantTable->SetVector(g_pd3dDevice, g_diffuseShaderMtrlHandle, &diffuseMtrl);
    g_pDiffuseShaderConstantTable->SetVector(g_pd3dDevice, g_diffuseShaderLightIntHandle, &diffuseLightInt);
    g_pDiffuseShaderConstantTable->SetDefaults(g_pd3dDevice);
}

void config_specular_shader()
{
    D3DXVECTOR4 specularMtrl(g_pMaterial->Specular.r, g_pMaterial->Specular.g, g_pMaterial->Specular.b, g_pMaterial->Specular.a);
    D3DXVECTOR4 specularLightInt(g_pLight->Specular.r, g_pLight->Specular.g, g_pLight->Specular.b, g_pLight->Specular.a);

    g_pSpecularShaderConstantTable->SetVector(g_pd3dDevice, g_specularShaderMtrlHandle, &specularMtrl);
    g_pSpecularShaderConstantTable->SetVector(g_pd3dDevice, g_specularShaderLightIntHandle, &specularLightInt);
    g_pSpecularShaderConstantTable->SetDefaults(g_pd3dDevice);
}

void config_combo_shader()
{
    D3DXVECTOR4 ambientMtrl(g_pMaterial->Ambient.r, g_pMaterial->Ambient.g, g_pMaterial->Ambient.b, g_pMaterial->Ambient.a);
    D3DXVECTOR4 diffuseMtrl(g_pMaterial->Diffuse.r, g_pMaterial->Diffuse.g, g_pMaterial->Diffuse.b, g_pMaterial->Diffuse.a);
    D3DXVECTOR4 specularMtrl(g_pMaterial->Specular.r, g_pMaterial->Specular.g, g_pMaterial->Specular.b, g_pMaterial->Specular.a);
    D3DXVECTOR4 ambientLightInt(g_pLight->Ambient.r, g_pLight->Ambient.g, g_pLight->Ambient.b, g_pLight->Ambient.a);
    D3DXVECTOR4 diffuseLightInt(g_pLight->Diffuse.r, g_pLight->Diffuse.g, g_pLight->Diffuse.b, g_pLight->Diffuse.a);
    D3DXVECTOR4 specularLightInt(g_pLight->Specular.r, g_pLight->Specular.g, g_pLight->Specular.b, g_pLight->Specular.a);

    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderAmbientMtrlHandle, &ambientMtrl);
    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderDiffuseMtrlHandle, &diffuseMtrl);
    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderSpecularMtrlHandle, &specularMtrl);
    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderAmbientLightIntHandle, &ambientLightInt);
    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderDiffuseLightIntHandle, &diffuseLightInt);
    g_pComboShaderConstantTable->SetVector(g_pd3dDevice, g_comboShaderSpecularLightIntHandle, &specularLightInt);
    g_pComboShaderConstantTable->SetDefaults(g_pd3dDevice);
}

void camera()
{
    D3DXVECTOR3 position(3.0f, 3.0f, -3.0f);
    D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
    D3DXMATRIX view;
    D3DXMatrixLookAtLH(&view, &position, &target, &up);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &view);

    g_viewMatrix = view;
    g_viewVector = D3DXVECTOR4(position.x - target.x, position.y - target.y, position.z - target.z, 0.0f);
}

void rotate()
{
    g_rotate += ROTATE_DELTA;
    if(g_rotate > MAX_ROTATE) {
        g_rotate -= MAX_ROTATE;
    }
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
    case VK_KEY_1:
        g_pCurrentShader = g_pAmbientShader;
        break;

    case VK_KEY_2:
        g_pCurrentShader = g_pDiffuseShader;
        break;

    case VK_KEY_3:
        g_pCurrentShader = g_pSpecularShader;
        break;

    case VK_KEY_4:
        g_pCurrentShader = g_pComboShader;
        break;

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