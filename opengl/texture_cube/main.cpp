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

#include <texture/texture_loader.h>
#include <application/application.h>

#include <config_opengl_texture_cube.h>

#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE

class tcapplication : public application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds delta) override;
    void draw() override;

    void info() override;
    void keyboard(SDL_Event const& e);

private:
    bool load_texture(const std::string& file, size_t num);

private:
    GLuint m_texture[7];
    std::size_t m_textureIndex;

    GLdouble g_xrot;
    GLdouble g_yrot;
    GLdouble g_zrot;

    bool m_blending;
    keyboard_press_guard m_bl_functor;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h),
    m_textureIndex(0),
    g_xrot(0),
    g_yrot(0),
    g_zrot(0),
    m_blending(false),
    m_bl_functor(SDLK_b, [this](){
        if(m_blending) {
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
        }
        else {
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
        }
        m_blending = !m_blending;
    })
{
}

void tcapplication::init()
{
    std::vector<std::string> const textureNameContainer{
        "opengl.bmp",
        "gamedev.bmp",
        "goblet.bmp",
        "box.bmp",
        "fire.bmp",
        "stone.bmp",
        "steel.bmp"
    };

    std::string const textureFile(PARENT_DIRECTORY + std::string("data/"));

    for(size_t i = 0; i < textureNameContainer.size(); i++) {
        if(!load_texture(textureFile + textureNameContainer[i], i)) {
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

void tcapplication::resize(std::size_t w, std::size_t h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble) w / (GLdouble) h, 0.1, 100.0);
}

void tcapplication::update(std::chrono::microseconds delta)
{
    double const count = static_cast<double>(delta.count());
    double constexpr timestep = 16000;

    g_xrot += 0.3 * count / timestep;
    g_yrot += 0.2 * count / timestep;
    g_zrot += 0.4 * count / timestep;
}

void tcapplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -5.0);

    glRotated(g_xrot, 1.0, 0.0, 0.0);
    glRotated(g_yrot, 0.0, 1.0, 0.0);
    glRotated(g_zrot, 0.0, 0.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, m_texture[m_textureIndex]);

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
}

void tcapplication::info()
{
    std::cout << "Press 'B' to change the blending\n";
    std::cout << "Press '1', '2', '3', '4' to change textuire\n";
}

void tcapplication::keyboard(SDL_Event const& e)
{
    switch (e.key.keysym.sym)
    {
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
            m_textureIndex = static_cast<std::size_t>(e.key.keysym.sym - '1');
            break;

        case SDLK_b:
            m_bl_functor(e);
            break;

        default:
            break;
    }
}

bool tcapplication::load_texture(const std::string& file, size_t num)
{
    bool status = false;

    texture::loader loader;
    loader.load(file.c_str());

    if(loader.isLoaded()) {
        status = true;

        glGenTextures(1, &m_texture[num]);
        glBindTexture(GL_TEXTURE_2D, m_texture[num]);

        glTexImage2D(GL_TEXTURE_2D, 0, 3, loader.getWidth(), loader.getHeight(),
            0, GL_BGR, GL_UNSIGNED_BYTE, loader.getData());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, loader.getWidth(), loader.getHeight(),
            GL_BGR, GL_UNSIGNED_BYTE, loader.getData());

        int g_nMaxAnisotropy;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &g_nMaxAnisotropy);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, g_nMaxAnisotropy);
    }

    return status;
}

int main( int argc, char* argv[] )
{
    tcapplication app(argc, argv, 640, 480);

    return app.run();
}
