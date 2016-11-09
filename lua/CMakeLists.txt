#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  LUA\n")

SET(PROJECT_FOLDER "lua")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

#ADD_SUBDIRECTORY(alloc)
ADD_SUBDIRECTORY(load_dll)

MESSAGE("")
