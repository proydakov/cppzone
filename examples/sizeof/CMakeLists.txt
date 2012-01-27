#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for SIZEOF  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  sizeof  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_sizeof ${SOURCE_FILES})
