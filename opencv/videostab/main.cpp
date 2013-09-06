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

#include <memory>
#include <cstdlib>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videostab/videostab.hpp>

class CameraFrameSource : public cv::videostab::IFrameSource
{
public:
    CameraFrameSource(int device) {
        m_capture.reset(new cv::VideoCapture(device));
    }

    bool isOpened() {
        if(m_capture.get()) {
            return m_capture->isOpened();
        }
        return false;
    }

    void reset( ) {
    }

    cv::Mat nextFrame( ) {
        m_capture->read(m_buffer);
        cv::imshow("cam", m_buffer);
        return m_buffer;
    }

private:
    std::auto_ptr<cv::VideoCapture> m_capture;
    cv::Mat m_buffer;
};

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    CameraFrameSource source(0);

    if (!source.isOpened()) {
        std::cerr << "cannot open camera";
        return EXIT_FAILURE;
    }

    std::auto_ptr<cv::videostab::StabilizerBase> stabilizer;
    std::auto_ptr<cv::videostab::GaussianMotionFilter> motionFilter;

    motionFilter.reset(new cv::videostab::GaussianMotionFilter());
    cv::videostab::OnePassStabilizer* opstabilizer = new cv::videostab::OnePassStabilizer();
    opstabilizer->setMotionFilter(motionFilter.get());
    stabilizer.reset(opstabilizer);

    std::auto_ptr<cv::videostab::PyrLkRobustMotionEstimator> motionEstimator;
    motionEstimator.reset(new cv::videostab::PyrLkRobustMotionEstimator());
    motionEstimator->setMotionModel(cv::videostab::TRANSLATION);

    stabilizer->setBorderMode(cv::BORDER_CONSTANT);

    while (true) {
        cv::imshow("stab", source.nextFrame());
        if (cv::waitKey(27) >= 0) {
            break;
        }
    }
    return EXIT_SUCCESS;
}
