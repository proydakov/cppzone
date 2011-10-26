#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for DISTANCE  ON  THE  TREE  TASK
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "create  distance  on  the  tree  task")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(task_distance_on_the_tree ${SOURCE_FILES})
