#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for STRINGS  EXAMPLE
#=============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "create  strings  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

SET(HEADER_FILES
        operations_with_strings.h)

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_strings ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(example_strings benchmark)
TARGET_LINK_LIBRARIES(example_strings benchmark)
