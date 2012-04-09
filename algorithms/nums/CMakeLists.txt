#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  nums  algorithms")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BENCHMARK_DIRECTORY})

# find Boost
FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
    operations_with_numbers.h)

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(algorithms_nums ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(algorithms_nums benchmark)
TARGET_LINK_LIBRARIES(algorithms_nums benchmark)
