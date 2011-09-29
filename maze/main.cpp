/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

#include <iostream>
#include <opencv2/highgui/highgui.hpp>

#include <generator.h>

// control key
static const int generate_button_key = 103;
static const int clear_button_key = 32;
static const int exit_button_key = 27;
static const int quit_button_key = 113;

// map configure
static const int map_width = 640;
static const int map_height = 480;

static const int map_way = 0;
static const int map_wall = 160;

void clear_matrix(cv::Mat& map)
{
    fill_matrix(map, map_way);
}

int main(int argc, char** argv)
{ 
    (void)argc;
    (void)argv;
    
    cv::Mat map(map_height, map_width, CV_8UC1, cv::Scalar(map_way));
    
    cv::namedWindow("maze", CV_WINDOW_FULLSCREEN); 
    cv::imshow("maze", map );
    
    while(1) {
        int key = cv::waitKey();
        if(key == exit_button_key || key == quit_button_key)
            break;
        else if(key == generate_button_key) {
            //create_matrix_border(map, 10, map_wall);
            generare_maze(map, map_wall);
            cv::imshow("maze", map );
        }
        else if(key == clear_button_key) {
            clear_matrix(map);
            cv::imshow("maze", map);
        }
        else
            std::cout << key << std::endl;
    }
    cv::destroyAllWindows();
    return 0;
}
