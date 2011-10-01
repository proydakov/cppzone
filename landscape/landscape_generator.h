/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

namespace cv {
class Mat;
};

void generator_mount(cv::Mat& matrix, int hill_type, int lowland_type, int pixel_size = 10);
void two_dimensional_random_generator(cv::Mat& matrix, int wall_type, int way_type, int pixel_size = 10);
