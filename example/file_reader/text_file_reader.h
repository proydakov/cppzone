/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_TEXT_FILE_READER
#define I_TEXT_FILE_READER

#include <fstream>
#include <string>
#include <list>

typedef std::list<std::string> data_list;

class text_file_reader
{
public:
    text_file_reader() {}

    bool download_from_file(const std::string& file_name, data_list& data)
    {
        std::string temp_string;
        std::ifstream stream(file_name.c_str(), std::ios_base::in);
        if (!stream.is_open()) {
            std::cerr << "Can't open file" << std::endl;
            return false;
        }
        while(1) {
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
