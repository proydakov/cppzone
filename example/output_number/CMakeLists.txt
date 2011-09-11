###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# Cmake for MEMORY  LEAK  EXAMPLE
###############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE  RECURSIVE  OUTPUT  NUMBER  EXAMPLE\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_output_number ${SOURCE_FILES})
