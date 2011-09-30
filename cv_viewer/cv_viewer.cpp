/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <cv_viewer.h>

namespace cv_viewer {

// control key
static const int generate_button_key = 103;
static const int clear_button_key = 32;
static const int exit_button_key = 27;
static const int quit_button_key = 113;

void fill_matrix(cv::Mat& matrix, int fill_type)
{
    for(int i = 0; i < matrix.cols * matrix.rows; ++i)
        *(matrix.data + i) = fill_type;
}

void clear_matrix(cv::Mat& map, int data)
{
    fill_matrix(map, data);
}

cv_viewer::cv_viewer(int height, int width, int clearance, int occupied_space, int pixel_size) :
    m_height(height),
    m_width(width),
    m_clearance(clearance),
    m_occupied_space(occupied_space),
    m_pixel_size(pixel_size)
{
    m_map = new cv::Mat(m_height, m_width, CV_8UC1, cv::Scalar(clearance));
}

cv_viewer::~cv_viewer()
{
    delete m_map;
}

void cv_viewer::set_task(task_set& task)
{
    m_task.clear();
    task_set::const_iterator endIt = task.end();
    int i = 0;
    for(task_set::const_iterator it = task.begin(); it != endIt; ++it) {
        m_task.insert(task_pair(i, *it));
        ++i;
    }
}

void cv_viewer::start(std::string window_title)
{ 
    cv::namedWindow(window_title, CV_WINDOW_FULLSCREEN | CV_WINDOW_AUTOSIZE); 
    int generator_type = 0;
    int size = m_task.size();
    if(size)
        cv::createTrackbar("Generator type", window_title, &generator_type, size);
    cv::imshow(window_title, *m_map );
    
    while(1) {
        int key = cv::waitKey();
        if(key == exit_button_key || key == quit_button_key)
            break;
        else if(key == generate_button_key) {
            cv_viewer::task_map::const_iterator it = m_task.find(generator_type);
            if(it != m_task.end())
                (*it).second(*m_map, m_clearance, m_occupied_space, m_pixel_size);
            cv::imshow(window_title, *m_map );
        }
        else if(key == clear_button_key) {
            clear_matrix(*m_map, m_clearance);
            cv::imshow(window_title, *m_map);
        }
        else
            std::cout << "Detected new key:  " << key << std::endl;
    }
}

void cv_viewer::stop()
{
    cv::destroyAllWindows();
}

} // close cv_viewer
