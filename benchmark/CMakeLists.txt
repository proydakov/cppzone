#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for BENCHMARK  LIBRARY
#==============================================================================

MESSAGE(STATUS "CREATE  BENCHMARK  LIBRARY\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost COMPONENTS date_time REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
        benchmark.h)

SET(SOURCE_FILES
        benchmark.cpp)

ADD_LIBRARY(benchmark ${FILES} ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(benchmark ${Boost_DATE_TIME_LIBRARY})
