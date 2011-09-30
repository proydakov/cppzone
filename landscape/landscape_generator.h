/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

namespace cv {
class Mat;
};

void fill_matrix(cv::Mat& matrix, int fill_type);
void two_dimensional_random_generator(cv::Mat& matrix, int wall_type, int way_type = 0, int pixel_size = 10);
