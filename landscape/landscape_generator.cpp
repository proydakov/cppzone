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
    int col = 5;
    
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

void draw_hexagon(cv::Mat& matrix, cv::Point& center, int side, int fill_type)
{
    cv::Point points[6];
    points[0] = cv::Point(center.x - side / 2, center.y - sqrt(3) / 2 * side);
    points[1] = cv::Point(center.x + side / 2, center.y - sqrt(3) / 2 * side);
    points[2] = cv::Point(center.x + side, center.y);
    points[3] = cv::Point(center.x + side / 2, center.y + sqrt(3) / 2 * side);
    points[4] = cv::Point(center.x - side / 2, center.y + sqrt(3) / 2 * side);
    points[5] = cv::Point(center.x - side, center.y);
    
    cv::fillConvexPoly(matrix, points, 6, cv::Scalar(fill_type));
}

void generator_hexagonal_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10)
{
    int side = pixel_size * 7;
    
    int width = matrix.cols + side;
    int height = matrix.rows + side;
    
    for(int b = 0; b < 2; ++b) {
        int delta_i = 0;
        int delta_j = 0;
        if(b) {
            delta_i = side * 3 / 2;
            delta_j = side * sqrt(3) / 2;
        }
        for(int i = 0; i < width; i += 3 * side) {
            for(int j = 0, k = 0; j < height; j += sqrt(3) * side, ++k) {    
                cv::Point point(i + delta_i, j + delta_j);  
                int color;
                if(b) {
                    if(k % 3 == 0)
                        color = (hill_type + lowland_type) / 2;
                    else if(k % 3 == 1)
                        color = hill_type;
                    else 
                        color = lowland_type;  
                }
                else {
                    if(k % 3 == 0)
                        color = hill_type;
                    else if(k % 3 == 1)
                        color = lowland_type;
                    else 
                        color = (hill_type + lowland_type) / 2;
                }
                draw_hexagon(matrix, point, side, color);
            }
        }
    }
}

void generator_chess_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size)
{
    int side = pixel_size * 10;
    
    int width = matrix.cols + side;
    int height = matrix.rows + side;
    
    for(int i = 0, ki = 0; i < width; i += side, ++ki) {
        for(int j = 0, kj = 0; j < height; j += side, ++kj) {
            cv::Point(i, j);
            cv::rectangle(matrix, cv::Rect(i, j, side, side),
                          cv::Scalar((ki + kj) % 2 ? hill_type : lowland_type), CV_FILLED);
        }
    }
}

void generator_triangle_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10)
{
    int side = pixel_size * 10;
    
    int width = matrix.cols + side;
    int height = matrix.rows + side;
    
    for(int i = 0, ki = 0; i < width; i += side / 2, ++ki) {
        for(int j = 0, kj = 0; j < height; j += side * sqrt(3) / 2, ++kj) {      
            int n = 3;
            cv::Point points[n];
            int color;
            if((ki + kj) % 2) {
                color = lowland_type;
                points[0] = cv::Point(i, j);
                points[1] = cv::Point(i - side / 2, j - side * sqrt(3) / 2);
                points[2] = cv::Point(i + side / 2, j - side * sqrt(3) / 2);
            }
            else {
                color = hill_type;
                points[0] = cv::Point(i - side / 2, j);
                points[1] = cv::Point(i + side / 2, j);
                points[2] = cv::Point(i, j - side * sqrt(3) / 2);
            }
            cv::fillConvexPoly(matrix, points, n, cv::Scalar(color));  
        }
    }
}
