#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  sort  algorithms")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

SET(SOURCE_FILES
   main.cpp
   )

I_ADD_EXECUTABLE(algorithms_sort_memory ${HEADER_FILES} ${SOURCE_FILES})
