#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for MEMORY  LEAK  EXAMPLE
#=============================================================================

MESSAGE(STATUS "create  memory  leak  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
        main.cpp)

ADD_EXECUTABLE(example_memory_leak ${SOURCE_FILES})
