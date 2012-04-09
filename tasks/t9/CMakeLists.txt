#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  t9  task")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(task_t9 ${SOURCE_FILES})
