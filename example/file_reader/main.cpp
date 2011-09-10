/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

#include <text_file_reader.h>
#include <file_reader_config.h>

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    text_file_reader reader;
    data_list list;
    if(reader.download_from_file(INPUT_FILE, list)) {
        list.sort();
        reader.written_in_file("output.txt", list);
    }
    else {
        std::cout << "Error download from file";
        return 0;
    }

    return 1;
}
