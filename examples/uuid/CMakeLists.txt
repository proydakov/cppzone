#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  UUID  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  uuid  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# find Boost
FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_uuid ${SOURCE_FILES})
