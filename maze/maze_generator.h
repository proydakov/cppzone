/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

namespace cv {
class Mat;
};

void simple_maze_generator(cv::Mat& matrix, int wall_type, int way_type = 0, int pixel_size = 10);
