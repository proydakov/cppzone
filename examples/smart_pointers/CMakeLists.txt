#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for SMART  POINTERS  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  smart  pointers  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_smart_pointers ${SOURCE_FILES})
