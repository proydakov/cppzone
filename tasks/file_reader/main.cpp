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
    return 0;
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
