#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for OUTPUT  NUMBER  TASK
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "create  output  number  task")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(task_output_number ${SOURCE_FILES})
