###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

PROJECT(I++)
MESSAGE(STATUS "CREATE  PROJECT  THEATER  OF  ROBOTS")

# temp setting
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

# build libraries type
IF(NOT BUILD_SHARED_LIBS)
    SET(BUILD_SHARED_LIBS ON)
ENDIF(NOT BUILD_SHARED_LIBS)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# call system info script
INCLUDE(${CMAKE_MODULE_PATH}/FindOS.cmake)

# add the include folder to the include path
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


