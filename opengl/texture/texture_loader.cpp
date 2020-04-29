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

#include "texture_loader.h"

#include <SDL2/SDL_surface.h>

using namespace texture;

bool get_ext(const std::string& file_name, std::string& ext)
{
    auto pos = file_name.rfind('.');
    if(pos == std::string::npos)
    {
        return false;
    }

    size_t size = file_name.size();
    ext.resize(size - static_cast<size_t>(pos));
    for(size_t i = pos, j = 0; i < size; ++i, ++j) {
        ext[j] = file_name[i];
    }
    return true;
}

struct loader::d
{
    d() : m_surface(0) { }
    ~d() { safe_unloading(); }

    void safe_unloading()
    {
        if(m_surface) {
            SDL_FreeSurface(m_surface);
            m_surface = 0;
        }
    }

    SDL_Surface* m_surface;
};

loader::loader() :
    m_d(new d)
{
}

loader::~loader()
{
}

bool loader::load(const std::string& file_name)
{
    std::string ext;
    if(!get_ext(file_name, ext)) {
        return false;
    }

    m_d->safe_unloading();

    if(ext == ".bmp") {
        m_d->m_surface = SDL_LoadBMP(file_name.c_str());
    }

    return m_d->m_surface ? true : false;
}

bool loader::isLoaded() const
{
    return m_d->m_surface ? true : false;
}

int loader::getWidth() const
{
    if(m_d->m_surface) {
        return m_d->m_surface->w;
    }
    return -1;
}

int loader::getHeight() const
{
    if(m_d->m_surface) {
        return m_d->m_surface->h;
    }
    return -1;
}

void* loader::getData() const
{
    if(m_d->m_surface) {
        return m_d->m_surface->pixels;
    }
    return 0;
}
