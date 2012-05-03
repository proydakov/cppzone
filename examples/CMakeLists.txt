#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  EXAMPLE  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  EXAMPLE\n")

SET(PROJECT_FOLDER "example")

ADD_SUBDIRECTORY(sizeof)
ADD_SUBDIRECTORY(smart_pointers)
ADD_SUBDIRECTORY(ublas)
ADD_SUBDIRECTORY(uuid)
ADD_SUBDIRECTORY(caller)

MESSAGE(STATUS "")
