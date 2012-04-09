#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  multithreading  load  cpu")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(multithreading_load_cpu ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(multithreading_load_cpu ${Boost_THREAD_LIBRARY})
