#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  MEMORY  LEAK  SYSTEM
#==============================================================================

MESSAGE(STATUS "create  memory  leak  system")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(system_memory_leak ${SOURCE_FILES})
