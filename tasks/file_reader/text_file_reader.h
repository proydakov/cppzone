/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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

#ifndef I_TEXT_FILE_READER
#define I_TEXT_FILE_READER

#include <fstream>
#include <string>
#include <list>

typedef std::list<std::string> data_list;

class text_file_master
{
public:
    text_file_master() {}

    bool download_from_file(const std::string& file_name, data_list& data)
    {
        std::ifstream stream(file_name.c_str(), std::ios::in);
        if (!stream.is_open()) {
            std::cerr << "Can't open file" << std::endl;
            return false;
        }
        std::string temp_string;
        while(true) {
            std::getline(stream, temp_string);
            if (stream.fail())
                break;
            data.push_back(temp_string);
        }
        return true;
    }

    bool written_in_file(const std::string& file_name, data_list& data)
    {
        std::ofstream stream(file_name.c_str(), std::ios::out);
        stream.clear();
        if (stream.fail())
            return false;
        for(data_list::const_iterator it = data.begin(); it != data.end(); ++it) {
            stream << it->c_str() << std::endl;
        }
        stream.close();
        return true;
    }
};

#endif // I_TEXT_FILE_READER
