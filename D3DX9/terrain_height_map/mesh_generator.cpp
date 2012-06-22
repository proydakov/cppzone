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

#include <geometry_loader.h>
#include <mesh_generator.h>

using namespace terrain;

struct mesh_generator::TerrainVertex
{
    TerrainVertex(const D3DXVECTOR3& iposition, const D3DXVECTOR3& inormal, const D3DXVECTOR2& itext1) :
        position(iposition),
        normal(inormal),
        text1(itext1)
    {
    }

    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 text1;

    static const DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

mesh_generator::mesh_generator()
{
}

mesh_generator::~mesh_generator()
{
}

bool mesh_generator::generate_mesh(LPDIRECT3DDEVICE9 pd3dDevice, geometry_loader* loader, FLOAT horizontalScale, FLOAT verticalScale, LPD3DXMESH* ppMesh)
{
    if(!generate_vertex(pd3dDevice, loader, horizontalScale, verticalScale, ppMesh)) {
        return false;
    }
    if(!generate_index(loader, ppMesh)) {
        return false;
    }
    if(!generate_attribute(loader, ppMesh)) {
        return false;
    }
    if(!generate_normal(ppMesh)) {
        return false;
    }
    return true;
}

bool mesh_generator::generate_vertex(LPDIRECT3DDEVICE9 pd3dDevice, geometry_loader* loader, FLOAT horizontalScale, FLOAT verticalScale, LPD3DXMESH* ppMesh)
{
    if(*ppMesh) {
        return false;
    }
    if(!loader->is_loaded()) {
        return false;
    }

    unsigned width = loader->get_width();
    unsigned height = loader->get_height();

    if(width == height && width == 1) {
        return false;
    }

    size_t num_verts = width * height;
    size_t num_faces = (width - 1) * (height - 1) * 2;

    D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
    D3DXDeclaratorFromFVF(TerrainVertex::FVF, elems);
    D3DXCreateMesh(num_faces, num_verts, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT, elems, pd3dDevice, ppMesh);

    TerrainVertex* vertex = 0;
    (*ppMesh)->LockVertexBuffer(0,(void**)&vertex);

    for(size_t h = 0; h < height; ++h) {
        for(size_t w = 0; w < width; ++w) {

            size_t index = h * width + w;

            FLOAT x = static_cast<FLOAT>(w) * horizontalScale;
            FLOAT y = static_cast<FLOAT>(loader->get_element_height(w, h)) * verticalScale;
            FLOAT z = static_cast<FLOAT>(h) * horizontalScale;

            FLOAT u = static_cast<FLOAT>(w) / width;
            FLOAT v = static_cast<FLOAT>(h) / height;

            D3DXVECTOR3 position(x, y, z);
            D3DXVECTOR3 normal(0, 0, 0);
            D3DXVECTOR2 texture(u, v);

            vertex[index].position = position;
            vertex[index].normal   = normal;
            vertex[index].text1    = texture;
        }
    }
    (*ppMesh)->UnlockVertexBuffer();

    return true;
}

bool mesh_generator::generate_index(geometry_loader* loader, LPD3DXMESH* ppMesh)
{
    unsigned width = loader->get_width();
    unsigned height = loader->get_height();

    size_t calc_width = width - 1;
    size_t calc_height = height - 1;

    DWORD* index = 0;
    (*ppMesh)->LockIndexBuffer(0, (void**)&index);

    size_t current_index = 0;
    for(size_t h = 0; h < calc_height; ++h) {
        for(size_t w = 0; w < calc_width; ++w) {

            DWORD i0 = width * h + w;
            DWORD i1 = width * h + w + 1;
            DWORD i2 = width * (h + 1) + w;
            DWORD i3 = width * (h + 1) + w + 1;

            index[current_index + 0] = i0;
            index[current_index + 1] = i3;
            index[current_index + 2] = i1;
            index[current_index + 3] = i0;
            index[current_index + 4] = i2;
            index[current_index + 5] = i3;

            current_index += 6;
        }
    }
    (*ppMesh)->UnlockIndexBuffer();

    return true;
}

bool mesh_generator::generate_attribute(geometry_loader* loader, LPD3DXMESH* ppMesh)
{
    unsigned width = loader->get_width();
    unsigned height = loader->get_height();

    size_t num_faces = (width - 1) * (height - 1) * 2;

    DWORD* attBuffer = 0;
    (*ppMesh)->LockAttributeBuffer(0, &attBuffer);

    for(size_t i = 0; i < num_faces; ++i) {
        attBuffer[i] = 0;
    }
    (*ppMesh)->UnlockAttributeBuffer();

    return true;
}

bool mesh_generator::generate_normal(LPD3DXMESH* ppMesh)
{
    D3DXComputeNormals((*ppMesh), NULL);

    return true;
}