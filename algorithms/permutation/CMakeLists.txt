#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  PERMUTATION  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  permutation  algorithms")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(algorithms_permutation ${SOURCE_FILES})
