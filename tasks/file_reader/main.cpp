/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

#include <text_file_reader.h>
#include <file_reader_config.h>

void print_list(data_list& list);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    text_file_master master;
    data_list list;
    if(master.download_from_file(INPUT_FILE, list)) {
        print_list(list);
        list.sort();
        print_list(list);
        master.written_in_file("output.txt", list);
    }
    else {
        std::cout << "Error download from file";
        return 0;
    }
    return 1;
}

void print_list(data_list& list)
{
    std::string string("------------------------");
    std::cout << string << std::endl;
    data_list::const_iterator endIt = list.end();
    for(data_list::const_iterator it = list.begin(); it != endIt; ++it) {
        std::cout << *it << std::endl;
    }
    std::cout << string << std::endl;
}
