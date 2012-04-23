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
