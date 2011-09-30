/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

#include <ctime>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/core/core_c.h>

void set_matrix_element(cv::Mat& matrix, int row, int col, int data)
{
    *(matrix.data + row * matrix.cols + col) = data;
}

void fill_matrix(cv::Mat& matrix, int fill_type)
{
    for(int i = 0; i < matrix.cols * matrix.rows; ++i)
        *(matrix.data + i) = fill_type;
}
/*
void create_matrix_border(cv::Mat& matrix, int border_size, int border_type)
{
    for(int i = 0; i < matrix.cols; ++i) {
        for(int j = 0; j < border_size; ++j) {
            set_matrix_element(matrix, j, i, border_type);
            set_matrix_element(matrix, matrix.rows - j - 1, i, border_type);
        } 
    }
    for(int i = 0; i < matrix.rows; ++i) {
        for(int j = 0; j < border_size; ++j) {
            set_matrix_element(matrix, i, j, border_type);
            set_matrix_element(matrix, i, matrix.cols - j - 1, border_type);
        }
    }
}
*/

void two_dimensional_random_generator(cv::Mat& matrix, int wall_type, int way_type, int pixel_size)
{
    fill_matrix(matrix, way_type);
    
    srand(time(NULL));
    for(int i = 0; i  < matrix.cols; i += pixel_size) {
        for(int j = 0; j < matrix.rows; j += pixel_size) {
            if(rand() % 2) {
                cv::rectangle(matrix, cv::Rect(i, j, pixel_size, pixel_size), cv::Scalar(wall_type), CV_FILLED);
            }
        }
    }
}

void simple_maze_generator(cv::Mat& matrix, int wall_type, int way_type, int pixel_size)
{
    fill_matrix(matrix, way_type);
    
    
}
