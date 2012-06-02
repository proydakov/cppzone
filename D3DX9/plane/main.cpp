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

#include <d3dx9math.h>

int main()
{
    D3DXVECTOR3 normal1(1, 0, 0);
    D3DXVECTOR3 point1(10, 0, 0);
    D3DXPLANE plane1;
    
    D3DXPlaneFromPointNormal(&plane1, &point1, &normal1);

    ///////////////////////////////////////////////////////////////////////////

    D3DXVECTOR3 normal2(0, 1, 1);
    D3DXVECTOR3 point2(0, 0, 1);
    D3DXPLANE plane2;
    D3DXPLANE plane2normal;
    
    D3DXPlaneFromPointNormal(&plane2, &point2, &normal2);
    D3DXPlaneNormalize(&plane2normal, &plane2);

    ///////////////////////////////////////////////////////////////////////////

    D3DXVECTOR3 normal3(1, 1, 1);
    D3DXVECTOR3 point3(1, 0, 0);
    D3DXPLANE plane3;
    D3DXPLANE plane3normal;

    D3DXPlaneFromPointNormal(&plane3, &point3, &normal3);
    D3DXPlaneNormalize(&plane3normal, &plane3);

    ///////////////////////////////////////////////////////////////////////////

    D3DXVECTOR3 p1(1, 0, 0);
    D3DXVECTOR3 p2(0, 1, 0);
    D3DXVECTOR3 p3(0, 0, 1);
    D3DXPLANE plane4;
    D3DXPLANE plane4normal;

    D3DXPlaneFromPoints(&plane4, &p3, &p1, &p2);
    D3DXPlaneNormalize(&plane4normal, &plane4);

    ///////////////////////////////////////////////////////////////////////////

    D3DXVECTOR3 pointBackward(2, 0, 0);
    D3DXVECTOR3 pointOn(10, 0, 0);
    D3DXVECTOR3 pointForward(12, 0, 0);
    D3DXPLANE plane(1, 0, 0, -10);

    FLOAT distanceForward = D3DXPlaneDotCoord(&plane, &pointForward);
    FLOAT distanceOn = D3DXPlaneDotCoord(&plane, &pointOn);
    FLOAT distanceBackward = D3DXPlaneDotCoord(&plane, &pointBackward);
    
    (void)distanceForward;
    (void)distanceOn;
    (void)distanceBackward;

    return 0;
}