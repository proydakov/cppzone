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
#include <iostream>

#include <GL/glut.h>

#include <texture/texture_loader.h>

#include <calc_fps.h>
#include <config_opengl_texture_cube.h>

#ifdef _MSC_VER
#   define GL_BGR 0x80E0
#endif // _MSC_VER

#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE

calc_fps g_fps;
unsigned g_last_fps = 0;

const std::string COMMENT = "Press ESC for exit.";

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

const int CYCLE_TIME = 25;
const int ESCAPE     = 27;

const unsigned TEXTURE_NUM = 4;

GLdouble g_xrot = 0;
GLdouble g_yrot = 0;
GLdouble g_zrot = 0;

GLuint g_texture[TEXTURE_NUM];
unsigned g_textureIndex = 0;

bool g_blending = false;

bool loadGLTexture(const std::string& file, size_t num)
{
    bool status = false;

    texture::loader loader;
    loader.load(file.c_str());

    if(loader.isLoaded()) {
	    status = true;

        glGenTextures(1, &g_texture[num]);
	    glBindTexture(GL_TEXTURE_2D, g_texture[num]);

	    glTexImage2D(GL_TEXTURE_2D, 0, 3, loader.getWidth(), loader.getHeight(), 
            0, GL_BGR, GL_UNSIGNED_BYTE, loader.getData());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, loader.getWidth(), loader.getHeight(),
            GL_BGR, GL_UNSIGNED_BYTE, loader.getData());
    }

    int g_nMaxAnisotropy;
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nMaxAnisotropy);

    return status;
}

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void fps()
{
    g_fps.calc();
    unsigned fps = g_fps.get_fps();
    if(fps != g_last_fps) {
        std::cout << "FPS : " << fps << std::endl;
        g_last_fps = fps;
    }
}

void init()
{
    std::string textureOpenGL("opengl.bmp");
    std::string textureGamedev("gamedev.bmp");
    std::string textureBox("box.bmp");
    std::string textureGobletBox("goblet.bmp");
    
    std::vector<std::string> textureNameContainer;
    textureNameContainer.push_back(textureOpenGL);
    textureNameContainer.push_back(textureGamedev);
    textureNameContainer.push_back(textureBox);
    textureNameContainer.push_back(textureGobletBox);

    std::string textureFile(PARENT_DIRECTORY + std::string("data/"));
    
    for(size_t i = 0; i < textureNameContainer.size(); ++i) {
        if(!loadGLTexture(textureFile + textureNameContainer[i], i)) {
            std::cerr << "Error loading texture" << std::endl;
            exit(1);
        }
    }

    glClearColor(0.0, 0.0, 0.0, 0.5);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glColor4d(1.0, 1.0, 1.0, 0.5);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -5.0);

    glRotated(g_xrot, 1.0, 0.0, 0.0);
    glRotated(g_yrot, 0.0, 1.0, 0.0);
    glRotated(g_zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, g_texture[g_textureIndex]);

    GLdouble verticesSquare[8][3] = { 
        { 1.0, -1.0, -1.0}, { 1.0, 1.0, -1.0}, { 1.0, -1.0, 1.0}, { 1.0, 1.0, 1.0}, 
        {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0}
    };

    glBegin(GL_QUADS);
    {
        // Front Face
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[0]);
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[2]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[3]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[1]);

        // Back Face
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[4]);
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[6]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[7]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[5]);

        // Top Face
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[2]);
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[6]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[7]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[3]);

        // Bottom Face
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[0]);
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[4]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[5]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[1]);

        // Right face
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[1]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[3]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[7]);
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[5]);

        // Left Face
        glTexCoord2d(0.0, 0.0); glVertex3dv(verticesSquare[0]);
        glTexCoord2d(0.0, 1.0); glVertex3dv(verticesSquare[2]);
        glTexCoord2d(1.0, 1.0); glVertex3dv(verticesSquare[6]);
        glTexCoord2d(1.0, 0.0); glVertex3dv(verticesSquare[4]);
    }
    glEnd();

    glFlush();
    glutSwapBuffers();

    fps();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble) w / (GLdouble) h, 0.1, 100.0);
}

void renderCycle()
{
    glutPostRedisplay();
}

void updateDataCycle(int value)
{
    g_xrot += 0.3;
    g_yrot += 0.2;
    g_zrot += 0.4;
    
    glutTimerFunc(CYCLE_TIME, updateDataCycle, value);
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case ESCAPE:
            exit(0);
            break;

        case '1':
            g_textureIndex = 0;
            break;

        case '2':
            g_textureIndex = 1;
            break;

        case '3':
            g_textureIndex = 2;
            break;

        case '4':
            g_textureIndex = 3;
            break;

        case 'b':
            if(g_blending) {
                glEnable(GL_DEPTH_TEST);
                glDisable(GL_BLEND);
            }
            else {
                glEnable(GL_BLEND);
                glDisable(GL_DEPTH_TEST);
            }
            g_blending = !g_blending;
            break;

        default:
            break;
    }
}

int main( int argc, char **argv )
{
    info();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(renderCycle);
    glutTimerFunc(CYCLE_TIME, updateDataCycle, 0);
    glutMainLoop();

    return 0;
}
