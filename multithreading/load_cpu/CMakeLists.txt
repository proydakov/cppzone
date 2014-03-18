#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  load  cpu  multithreading")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS system thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(multithreading_load_cpu ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(multithreading_load_cpu ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(multithreading_load_cpu ${Boost_SYSTEM_LIBRARY})
