#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  ublas  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(example_ublas ${SOURCE_FILES})
