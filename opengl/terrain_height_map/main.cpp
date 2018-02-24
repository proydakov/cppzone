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
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#include <common/iglut.h>

#include <terrain.h>

#include <config_terrain_height_map.h>

struct color_rgb
{
    color_rgb() : r(0), g(0) ,b(0) {}
    color_rgb(GLdouble ir, GLdouble ig, GLdouble ib) : r(ir), g(ig) ,b(ib) {}

    GLdouble r;
    GLdouble g;
    GLdouble b;
};

//-----------------------------------------------------------------------------
// Globals variables and definitions
//-----------------------------------------------------------------------------

const std::string COMMENT =
"Press 1-7 for the changes in the quality of parts.\n\
Press t or T to change the fill type.\n\
Press ESC for exit.\n";

static const int ESCAPE = 27;


static bool g_draw_solid = false;


static const int MAP_SIZE = 256;
static const int WORLD_SIZE_KOEF = 256;
static const GLdouble ROTATION_VERTICAL_DELTA   = 0.05;
static const GLdouble ROTATION_HORIZONTAL_DELTA = 0.05;
static const GLdouble MAX_ROTATION_HORIZONTAL = M_PI / 2 - 0.01;
static const GLdouble MIN_ROTATION_HORIZONTAL = 0.01;

static GLdouble g_rotationVertical = 0.01;
static GLdouble g_rotationHorizontal = 0.75;


static const unsigned MAX_TERRAIN_DETAILS = 7;
static const unsigned MIN_TERRAIN_DETAILS = 1;

terrain g_terrain;
unsigned g_terrainDetails = 4;
GLuint g_theTerrain[MAX_TERRAIN_DETAILS - MIN_TERRAIN_DETAILS + 1];
std::vector<color_rgb> g_theTerrainColor;

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

void info();
void init();
void init_map();
void init_color_buffer();

void render();
void draw_terrain(unsigned step);
void camera();

void cycle();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);

int main( int argc, char **argv )
{
    info();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(cycle);
    glutMainLoop();

    return 0;
}

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    init_map();
}

void init_map()
{
    std::string base_directory(DATA_DIRECTORY);
    std::string file_name(base_directory + "terrain.raw");

    unsigned side = MAP_SIZE;

    bool load = g_terrain.load_raw(file_name, side, side);
    if(!load) {
        std::cerr << "Fail load " << file_name << " map" << std::endl;
        exit(1);
    }

    init_color_buffer();

    for(unsigned i = MIN_TERRAIN_DETAILS; i <= MAX_TERRAIN_DETAILS; ++i) {
        g_theTerrain[i - 1] = glGenLists(1);
        glNewList(g_theTerrain[i - 1], GL_COMPILE);
        draw_terrain(i);
        glEndList();
    }
}

void init_color_buffer()
{
    g_theTerrainColor.resize(MAP_SIZE * MAP_SIZE);

    GLdouble max_height = g_terrain.get_max_height();
    unsigned width = g_terrain.get_width();
    unsigned height = g_terrain.get_height();

    for(unsigned i = 0; i < width - 1; ++i) {
        for(unsigned j = 0; j < height -1; ++j) {
            GLdouble current_height = g_terrain.get_element_height(i, j);
            GLdouble index = current_height / max_height;

            g_theTerrainColor[i + j * width] = color_rgb(1 - index, 0, index);
        }
    }
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if(g_draw_solid) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    glCallList(g_theTerrain[g_terrainDetails - 1]);

    glutSwapBuffers();
    glFlush();

    camera();
}

void draw_terrain(unsigned step)
{
    unsigned width = g_terrain.get_width();
    unsigned height = g_terrain.get_height();

    for(unsigned i = 0; i < width - step; i += step) {
        for(unsigned j = 0; j < height -step; j += step) {

            color_rgb color = g_theTerrainColor[i + j * width];
            glColor3d(color.r, color.g, color.b);

            glBegin(GL_POLYGON);
            {
                glVertex3i(       i,        j, g_terrain.get_element_height(       i,        j));
                glVertex3i(i + step,        j, g_terrain.get_element_height(i + step,        j));
                glVertex3i(i + step, j + step, g_terrain.get_element_height(i + step, j + step));
                glVertex3i(       i, j + step, g_terrain.get_element_height(       i, j + step));
            }
            glEnd();
        }
    }
}

void camera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLdouble x = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * cos(g_rotationVertical);
    GLdouble y = WORLD_SIZE_KOEF * sin(g_rotationHorizontal) * sin(g_rotationVertical);
    GLdouble z = WORLD_SIZE_KOEF * cos(g_rotationHorizontal);

    GLdouble width = g_terrain.get_width();
    GLdouble height = g_terrain.get_height();
    GLdouble max_height = g_terrain.get_max_height();

    gluLookAt(x, y, z, 0, 0, 0.0, 0.0, 0.0, 1.0);
    glTranslated(-width / 2, -height / 2, -max_height / 2);
}

void cycle()
{
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLdouble) w / (GLdouble) h, 1.0, 10000.0);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case 'A':
        case 'a':
            g_rotationVertical += ROTATION_VERTICAL_DELTA;
            break;

        case 'D':
        case 'd':
            g_rotationVertical -= ROTATION_VERTICAL_DELTA;
            break;

        case 'W':
        case 'w':
            g_rotationHorizontal -= ROTATION_HORIZONTAL_DELTA;
            if(g_rotationHorizontal < MIN_ROTATION_HORIZONTAL) {
                g_rotationHorizontal = MIN_ROTATION_HORIZONTAL;
            }
            break;

        case 'S':
        case 's':
            g_rotationHorizontal += ROTATION_HORIZONTAL_DELTA;
            if(g_rotationHorizontal > MAX_ROTATION_HORIZONTAL) {
                g_rotationHorizontal = MAX_ROTATION_HORIZONTAL;
            }
            break;

        case 'T':
        case 't':
            g_draw_solid = !g_draw_solid;
            break;

        case '1':
            g_terrainDetails = 1;
            break;

        case '2':
            g_terrainDetails = 2;
            break;

        case '3':
            g_terrainDetails = 3;
            break;

        case '4':
            g_terrainDetails = 4;
            break;

        case '5':
            g_terrainDetails = 5;
            break;

        case '6':
            g_terrainDetails = 6;
            break;

        case '7':
            g_terrainDetails = 7;
            break;

        case ESCAPE:
            exit(0);
            break;
    }
}
