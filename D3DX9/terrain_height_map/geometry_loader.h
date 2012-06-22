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

#ifndef I_TERRAIN_GEOMETRY_LOADER
#define I_TERRAIN_GEOMETRY_LOADER

#include <vector>

namespace terrain {

    class geometry_loader
    {
    public:
        typedef int heigh;

    public:
        geometry_loader();
        ~geometry_loader();

        bool load_raw(const std::string& file_name, unsigned width, unsigned height);
        bool is_loaded();

        heigh get_element_height(unsigned x, unsigned y) const;

        unsigned get_width() const;
        unsigned get_height() const;

        heigh get_min_height() const;
        heigh get_max_height() const;

    private:
        typedef unsigned char byte;

        std::vector<heigh> m_height_map;

        unsigned m_width;
        unsigned m_height;

        heigh m_min_height;
        heigh m_max_height;
    };
}

#endif // I_TERRAIN_GEOMETRY_LOADER