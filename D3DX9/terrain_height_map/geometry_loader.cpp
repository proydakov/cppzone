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

#include <cassert>
#include <fstream>

#include "geometry_loader.h"

using namespace terrain;

geometry_loader::geometry_loader() :
    m_width(0),
    m_height(0),
    m_min_height(0),
    m_max_height(0)
{
}

geometry_loader::~geometry_loader()
{
}

bool geometry_loader::load_raw(const std::string& file_name, unsigned width, unsigned height)
{
    size_t num_vertices = width * height;
    std::vector<byte> in(num_vertices);

    std::ifstream file;
    file.open(file_name.c_str(), std::ios_base::binary);
    if(!file.is_open()) {
        return false;
    }

    file.read(reinterpret_cast<char*>(&in[0]), in.size());
    file.close();

    m_min_height = in[0];
    m_max_height = in[0];

    m_height_map.resize(num_vertices);
    for(size_t i = 0; i < in.size(); i++) {
        m_height_map[i] = in[i];
        if(in[i] > m_max_height) {
            m_max_height = in[i];
        }
        if(in[i] < m_min_height) {
            m_min_height = in[i];
        }
    }
    m_width = width;
    m_height = height;

    return true;
}

bool geometry_loader::is_loaded()
{
    return m_width * m_height ? true : false;
}

geometry_loader::heigh geometry_loader::get_element_height(unsigned x, unsigned y) const
{
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);

    size_t position = y * m_width + x;
    return m_height_map[position];
}

geometry_loader::heigh geometry_loader::get_max_height() const
{
    return m_max_height;
}

unsigned geometry_loader::get_width() const
{
    return m_width;
}

unsigned geometry_loader::get_height() const
{
    return m_height;
}