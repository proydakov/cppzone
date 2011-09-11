###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# Cmake for SMART  POINTERS  EXAMPLE
###############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE  UUID  EXAMPLE\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_smart_pointers ${SOURCE_FILES})
