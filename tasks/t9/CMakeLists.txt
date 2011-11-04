#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for T9  TASK
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "create  t9  task")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(task_t9 ${SOURCE_FILES})
ADD_DEPENDENCIES(task_t9 benchmark)
TARGET_LINK_LIBRARIES(task_t9 benchmark)
