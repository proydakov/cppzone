###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# Cmake for THREAD  EXAMPLE
###############################################################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

MESSAGE(STATUS "CREATE  UUID  EXAMPLE\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_uuid ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(example_uuid ${Boost_THREAD_LIBRARY})
