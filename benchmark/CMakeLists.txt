#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  BENCHMARK  LIBRARY
#==============================================================================

MESSAGE(STATUS "CREATE  BENCHMARK  LIBRARY\n")

SET(PROJECT_FOLDER "libs")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS date_time REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
    clock.h
    )

SET(SOURCE_FILES
    clock.cpp
    )

ADD_LIBRARY(benchmark STATIC ${FILES} ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET benchmark PROPERTY FOLDER ${PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(benchmark ${Boost_DATE_TIME_LIBRARY})

MESSAGE(STATUS "")
