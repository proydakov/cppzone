/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <iostream>

#include <cv_viewer.h>

#include <maze_generator.h>

// map configure
static const int map_width = 640;
static const int map_height = 480;

static const int pixel_size = 4;

static const int map_way = 0;
static const int map_wall = 160;

int main(int argc, char** argv)
{ 
    (void)argc;
    (void)argv;
    
    cv_viewer::task_set task;
    task.insert(&simple_maze_generator);
    
    cv_viewer::cv_viewer viewer(map_height, map_width, map_way, map_wall, pixel_size);
    viewer.set_task(task);
    viewer.start("maze");
    viewer.stop();
    
    return 0;
}
