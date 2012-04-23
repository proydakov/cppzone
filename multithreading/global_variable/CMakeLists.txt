#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  global  variable  multithreading")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(multithreading_global_variable ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(multithreading_global_variable ${Boost_THREAD_LIBRARY})
