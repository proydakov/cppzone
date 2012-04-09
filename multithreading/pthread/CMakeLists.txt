#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  PTHREAD  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  multithreading  pthread")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_LIBRARY(PTHREAD pthread $ENV{LD_LIBRARY_PATH})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(multithreading_pthread ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(multithreading_pthread ${PTHREAD})
