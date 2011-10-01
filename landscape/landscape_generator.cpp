/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <ctime>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

int get_line_size(const cv::Point a, const cv::Point b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void fill_matrix(cv::Mat& matrix, int fill_type)
{
    for(int i = 0; i < matrix.cols * matrix.rows; ++i)
        *(matrix.data + i) = fill_type;
}

void generator_mounts(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10)
{  
    fill_matrix(matrix, lowland_type);
    
    srand(time(NULL));
    int col = rand() % 5 + 1;
    
    typedef std::multimap<int ,cv::Point> mount_map;
    typedef std::pair<int ,cv::Point> mount_pair;
    mount_map mounts;
    
    for(int i = 0; i < col; ++i) {
        int x = rand() % matrix.cols;
        int y = rand() % matrix.rows;
        int R = std::min(matrix.rows, matrix.cols) / (rand() % 5 + 4);
        mounts.insert(mount_pair(R ,cv::Point(x, y)));
    }
    
    for(int i = 0; i < matrix.cols; i += pixel_size) {
        for(int j = 0; j < matrix.rows; j += pixel_size) {
            cv::Point point(i, j);
            for(mount_map::const_iterator it = mounts.begin(); it != mounts.end(); ++it) {
                int length = get_line_size(it->second, point);
                
                int land_type = length ? hill_type * (1 - (double)length / it->first): hill_type;
                land_type += rand() % ((hill_type - lowland_type) / 35);
                if(length < it->first && *(matrix.data + j * matrix.cols + i) <= land_type) {
                    cv::rectangle(matrix, cv::Rect(i, j, pixel_size, pixel_size), cv::Scalar(land_type), CV_FILLED);
                }
            }
        }
    }
}

void two_dimensional_random_generator(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10)
{ 
    cv::Mat& map(matrix);
    
    srand(time(NULL));
    for(int i = 0; i  < map.cols; i += pixel_size) {
        for(int j = 0; j < map.rows; j += pixel_size) {
            int land_type = (hill_type - lowland_type) / (rand() % (hill_type / pixel_size) + 1);
            cv::rectangle(map, cv::Rect(i, j, pixel_size, pixel_size), cv::Scalar(land_type), CV_FILLED);
        }
    }
    matrix = map;
}
