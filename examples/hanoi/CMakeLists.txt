#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  HANOI  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  hanoi  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_hanoi ${SOURCE_FILES})
