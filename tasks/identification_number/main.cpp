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

#include <cmath>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include <identification_number_config.h>

typedef std::list<std::string> data_list;
typedef std::vector<double> equivalents;
typedef std::vector<equivalents> table_of_equivalents;

const int VOID_ELEMENT = 0;
const int FILLED_ELEMENT = 1;

class matrix
{
public:
    matrix();
    ~matrix();
    
    void create();
    void resize(int height, int width);
    int get_height();
    int get_width();
    
    void print();
    
    int* data;
    
private:
    int m_height;
    int m_width;
};

bool download_from_file(const std::string& file_name, data_list& data);
void converted_from_string_list_to_matrix(data_list& data, char void_symbol, matrix& matrix_data);
bool delimitation(matrix& matrix_data, int& left, int& right, int& up, int& down);
int identify_the_number_of_quarters_occupancy(matrix& matrix_data, table_of_equivalents& table, int& left_border, int& right_border, int& up_border, int& down_border);
int search_in_the_table_of_correspondences(table_of_equivalents& table, equivalents& data);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    // create table of equivalents
    table_of_equivalents the_table_of_equivalents;
    
    equivalents zero;
    zero.push_back(0.25);
    zero.push_back(0.25);
    zero.push_back(0.25);
    zero.push_back(0.25);
    
    equivalents one;
    one.push_back(0.2);
    one.push_back(0.5);
    one.push_back(0);
    one.push_back(0.3);
    
    equivalents two;
    two.push_back(0.2);
    two.push_back(0.3);
    two.push_back(0.31);
    two.push_back(0.19);
    
    equivalents three;
    three.push_back(0.15);
    three.push_back(0.35);
    three.push_back(0.15);
    three.push_back(0.35);
    
    equivalents four;
    four.push_back(0.33);
    four.push_back(0.33);
    four.push_back(0.33);
    four.push_back(0);
    
    equivalents five;
    five.push_back(0.36);
    five.push_back(0.15);
    five.push_back(0.14);
    five.push_back(0.35);
    
    equivalents six;
    six.push_back(0.29);
    six.push_back(0.12);
    six.push_back(0.3);
    six.push_back(0.29);
    
    equivalents seven;
    seven.push_back(0.19);
    seven.push_back(0.49);
    seven.push_back(0.3);
    seven.push_back(0.02);
    
    equivalents eight;
    eight.push_back(0.25);
    eight.push_back(0.25);
    eight.push_back(0.25);
    eight.push_back(0.25);
    
    equivalents nine;
    nine.push_back(0.29);
    nine.push_back(0.3);
    nine.push_back(0.12);
    nine.push_back(0.29);
    
    the_table_of_equivalents.push_back(zero);
    the_table_of_equivalents.push_back(one);
    the_table_of_equivalents.push_back(two);
    the_table_of_equivalents.push_back(three);
    the_table_of_equivalents.push_back(four);
    the_table_of_equivalents.push_back(five);
    the_table_of_equivalents.push_back(six);
    the_table_of_equivalents.push_back(seven);
    the_table_of_equivalents.push_back(eight);
    the_table_of_equivalents.push_back(nine);
    
    data_list data;
    if(download_from_file(INPUT_FILE_5, data)) {
        matrix image;
        converted_from_string_list_to_matrix(data, ' ', image);
        image.print();
        
        int image_num_left;
        int image_num_right;
        int image_num_up;
        int image_num_down;
        if(delimitation(image, image_num_left, image_num_right, image_num_up, image_num_down)) {
            int num = identify_the_number_of_quarters_occupancy(image, the_table_of_equivalents, image_num_left, image_num_right, image_num_up, image_num_down);
            std::cout << "Recognized number:  " << num << std::endl;
        }
        else {
            std::cout << "Could not find the image border";
            return 0;
        }
    }
    else {
        std::cout << "Error download from file";
        return 0;
    }
    return 0;
}

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

void converted_from_string_list_to_matrix(data_list& data, char void_symbol, matrix& matrix_data)
{
    int height = data.size();
    int width = data.begin()->size();
    
    matrix_data.resize(height, width);
    matrix_data.create();
    
    int j = 0;
    data_list::const_iterator endIt = data.end();
    for(data_list::const_iterator it = data.begin(); it != endIt; ++it) {
        for(int i = 0; i < width; ++i) {
            *(matrix_data.data + i + j * width) = (*it)[i] == void_symbol ? VOID_ELEMENT : FILLED_ELEMENT;
        }
        ++j;
    }
}

bool delimitation(matrix& matrix_data, int& left, int& right, int& up, int& down)
{
    bool res = false;
    
    int width = matrix_data.get_width();
    int height = matrix_data.get_height();
    
    left = width + 1;
    right = -1;
    up = height + 1;
    down = -1;
    
    for(int i = 0; i < height; ++i)
        for(int j = 0; j < width; ++j) {
            if(*(matrix_data.data + j + i * width) == FILLED_ELEMENT) {
                if(j < left)
                    left = j;
                if(j > right)
                    right = j;
                if(i < up)
                    up = i;
                if(i > down)
                    down = i;
                res = true;
            }
        }
    return res;
}

int identify_the_number_of_quarters_occupancy(matrix& matrix_data, table_of_equivalents& table, int& left_border, int& right_border, int& up_border, int& down_border)
{
    int width = matrix_data.get_width();
    
    int width_center = (up_border + down_border) / 2;
    int height_center = (left_border + right_border) / 2;
    
    int number_of_elements_in_the_first_quarter = 0;
    int number_of_elements_in_the_second_quarter = 0;
    int number_of_elements_in_the_third_quarter = 0;
    int number_of_elements_in_the_fourth_quarter = 0;
    
    // first quarter
    for(int i = up_border; i < height_center; ++i)
        for(int j = left_border; j < width_center; ++j) {
            if(*(matrix_data.data + j + i * width)) {
                ++number_of_elements_in_the_first_quarter;
            }
        }
    
    // second quarter
    for(int i = up_border; i < height_center; ++i)
        for(int j = width_center; j < right_border; ++j) {
            if(*(matrix_data.data + j + i * width)) {
                ++number_of_elements_in_the_second_quarter;
            }
        }
    
    // third quarter
    for(int i = height_center; i < down_border; ++i)
        for(int j = left_border; j < width_center; ++j) {
            if(*(matrix_data.data + j + i * width)) {
                ++number_of_elements_in_the_third_quarter;
            }
        }
    
    // fourth quarter
    for(int i = height_center; i < down_border; ++i)
        for(int j = width_center; j < right_border; ++j) {
            if(*(matrix_data.data + j + i * width)) {
                ++number_of_elements_in_the_fourth_quarter;
            }
        }
    
    int num = number_of_elements_in_the_first_quarter + 
            number_of_elements_in_the_second_quarter + 
            number_of_elements_in_the_third_quarter + 
            number_of_elements_in_the_fourth_quarter;
    
    equivalents data;
    data.push_back((double)number_of_elements_in_the_first_quarter / num);
    data.push_back((double)number_of_elements_in_the_second_quarter / num);
    data.push_back((double)number_of_elements_in_the_third_quarter / num);
    data.push_back((double)number_of_elements_in_the_fourth_quarter / num);
    
    return search_in_the_table_of_correspondences(table, data);
}

int search_in_the_table_of_correspondences(table_of_equivalents& table, equivalents& data)
{
    int index = 0;
    double delta = 100;
    double iteration = 0;
    
    for(unsigned i = 0; i < table.size(); ++i) {
        for(unsigned j = 0; j < data.size(); ++j) {
            iteration += std::fabs(table[i][j] - data[j]);
        }
        if(iteration < delta) {
            delta = iteration;
            index = i;
        }
        iteration = 0;
    }
    return index;
}

matrix::matrix() :
    m_height(0),
    m_width(0),
    data(0) {
}

matrix::~matrix()
{
    delete [] data;
}

void matrix::create() 
{
    delete [] data;
    data = new int[m_height * m_width];
}

void matrix::resize(int height, int width) 
{
    m_height = height;
    m_width = width;
}

int matrix::get_height()
{
    return m_height;
}

int matrix::get_width() 
{
    return m_width;
}

void matrix::print()
{
    for(int i = 0; i < m_height; ++i) {
        for(int j = 0; j < m_width; ++j) {
            std::cout << *(data + j + i * m_width);
        }
        std::cout << std::endl;
    }
}
