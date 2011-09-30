/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#ifndef I_CV_VIEWER_H
#define I_CV_VIEWER_H

#include <set>
#include <map>
#include <string>

namespace cv {
class Mat;
};

namespace cv_viewer {

typedef void (*prt_generator)(cv::Mat&, int, int, int);

typedef std::set<prt_generator> task_set;

class cv_viewer
{  
public:
    cv_viewer(int height, int width, int clearance, int occupied_space, int pixel_size);
    ~cv_viewer();
    
    void set_task(task_set& task);
    void start(std::string window_title);
    void stop();
    
private:
    int m_height;
    int m_width;
    int m_clearance;
    int m_occupied_space;
    int m_pixel_size;
    
    typedef std::map<int, prt_generator> task_map;
    typedef std::pair<int, prt_generator> task_pair;
    
    task_map m_task;
    
    cv::Mat* m_map;
};

} // close cv_viewer

#endif // I_CV_VIEWER_H
