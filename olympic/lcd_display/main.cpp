/*
 *  Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <list>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iostream>

struct num
{
    num(int size, const std::string& value) :
        size(size),
        value(value)
    {
    }

    int size;
    std::string value;
};

typedef std::vector<num> num_container;
typedef std::list<int>   digit_container;

class input_reader
{
public:
    static void read(num_container& data) {
        bool work = true;

        int size = 0;
        std::string value;

        while(work) {
            std::cin >> size;
            std::cin >> value;

            if(size == 0 && value == "0") {
                work = false;
            }
            else {
                data.push_back(num(size, value));
            }
        }
    }
};

class output_writer
{
public:
    static void write(num_container& data) {
        size_t size = data.size();
        for(size_t i = 0; i < size; i++) {
            write_num(data[i]);
            std::cout << std::endl;
        }
    }

    static void write_num(num& input) {
        digit_container value_by_digit;

        for(size_t i = 0; i < input.value.size(); i++) {
            char s = input.value[i];
            value_by_digit.push_back(atoi(&s));
        }

        int cycle = input.size * 2 + 3;
        int down_index = input.size * 2 + 2;
        int middle_index = input.size + 1;
        int up_index = 0;
        for(int vertival_index = 0; vertival_index < cycle; vertival_index++) {
            size_t size_index = 0;
            size_t size = value_by_digit.size();
            size_t end = size - 1;
            for(digit_container::const_iterator it = value_by_digit.begin(); it != value_by_digit.end(); it++, size_index++) {
                write_num_element(*it, input.size, vertival_index, up_index, middle_index, down_index);
                if(size_index != end) {
                    std::cout << " ";
                }
            }
            std::cout << "\n";
        }
    }

    static void write_num_element(int value, int size, int vertical_index, int up_index, int middel_index, int down_index) {
        std::stringstream sstream;
        switch(value) {
        case 0:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                for(int i = 0; i < size + 2; i++) {
                    sstream << " ";
                }
            }
            else if(vertical_index < down_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 1:
        {
            for(int i = 0; i < size; i++) {
                sstream << " ";
            }
            if(vertical_index == up_index) {
                sstream << "  ";
            }
            else if(vertical_index < middel_index) {
                sstream << " |";
            }
            else if(vertical_index == middel_index) {
                sstream << "  ";
            }
            else if(vertical_index < down_index) {
                sstream << " |";
            }
            else {
                sstream << "  ";
            }
        }
            break;

        case 2:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                sstream << "|";
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 3:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 4:
        {
            if(vertical_index == up_index) {
                for(int i = 0; i < size + 2; i++) {
                    sstream << " ";
                }
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                for(int i = 0; i < size + 2; i++) {
                    sstream << " ";
                }
            }
        }
            break;

        case 5:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 6:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 7:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                for(int i = 0; i < size + 2; i++) {
                    sstream << " ";
                }
            }
            else if(vertical_index < down_index) {
                for(int i = 0; i < size + 1; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                for(int i = 0; i < size + 2; i++) {
                    sstream << " ";
                }
            }
        }
            break;

        case 8:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;

        case 9:
        {
            if(vertical_index == up_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < middel_index) {
                sstream << "|";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else if(vertical_index == middel_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
            else if(vertical_index < down_index) {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << " ";
                }
                sstream << "|";
            }
            else {
                sstream << " ";
                for(int i = 0; i < size; i++) {
                    sstream << "-";
                }
                sstream << " ";
            }
        }
            break;
        }
        //std::cout << "value: " << value << " size: " << size << " vertical_index: " << vertical_index << " up_index: " << up_index << " middel_index: " << middel_index << " down_index: " << down_index;
        std::cout << sstream.str();
    }
};

int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);

    num_container data;
    input_reader::read(data);
    output_writer::write(data);

    return 0;
}
