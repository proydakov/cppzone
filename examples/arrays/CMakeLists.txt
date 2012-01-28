#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  ARRAY  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  arrays  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
        operations_with_arrays.h)

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_arrays ${HEADER_FILES} ${SOURCE_FILES})
