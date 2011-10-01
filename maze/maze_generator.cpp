/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <ctime>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>

void fill_matrix(cv::Mat& matrix, int fill_type)
{
    for(int i = 0; i < matrix.cols * matrix.rows; ++i)
        *(matrix.data + i) = fill_type;
}

void simple_maze_generator(cv::Mat& matrix, int wall_type, int way_type, int pixel_size)
{
    fill_matrix(matrix, way_type);
    
}
