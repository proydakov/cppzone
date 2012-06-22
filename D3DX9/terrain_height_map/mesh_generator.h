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

#ifndef I_TERRAIN_MESH_GENERATOR
#define I_TERRAIN_MESH_GENERATOR

#include <d3dx9mesh.h>

namespace terrain {
class geometry_loader;
}

namespace terrain {

    class mesh_generator
    {
    public:
        mesh_generator();
        ~mesh_generator();

        bool generate_mesh(LPDIRECT3DDEVICE9 pd3dDevice, geometry_loader* loader, FLOAT horizontalScale, FLOAT verticalScale, LPD3DXMESH* ppMesh);

    private:
        bool generate_vertex(LPDIRECT3DDEVICE9 pd3dDevice, geometry_loader* loader, FLOAT horizontalScale, FLOAT verticalScale, LPD3DXMESH* ppMesh);
        bool generate_index(geometry_loader* loader, LPD3DXMESH* ppMesh);
        bool generate_attribute(geometry_loader* loader, LPD3DXMESH* ppMesh);
        bool generate_normal(LPD3DXMESH* ppMesh);

    private:
        struct TerrainVertex;
    };
}

#endif // I_TERRAIN_MESH_GENERATOR