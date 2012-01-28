#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SORT  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  sort  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

SET(HEADER_FILES
        my_sort.h)

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_sort ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(example_sort benchmark)
TARGET_LINK_LIBRARIES(example_sort benchmark)
