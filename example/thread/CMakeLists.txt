#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for THREAD  EXAMPLE
#=============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE  THREAD  EXAMPLE\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
        load_cpu.cpp)

ADD_EXECUTABLE(example_thread_load_cpu ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(example_thread_load_cpu ${Boost_THREAD_LIBRARY})
