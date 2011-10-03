/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

namespace cv {
class Mat;
};

void generator_mounts(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10);
void two_dimensional_random_generator(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10);
void generator_hexagonal_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10);
void generator_chess_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10);
void generator_triangle_world(cv::Mat& matrix, int lowland_type, int hill_type, int pixel_size = 10);
