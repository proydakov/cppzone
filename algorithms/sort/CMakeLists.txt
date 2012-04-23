#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  sort  algorithms")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

SET(HEADER_FILES
    my_sort.h)

SET(SOURCE_FILES
   main.cpp)

I_ADD_EXECUTABLE(algorithms_sort ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(algorithms_sort benchmark)
TARGET_LINK_LIBRARIES(algorithms_sort benchmark)
