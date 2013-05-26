#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  EXAMPLE  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD BOOST EXAMPLE\n")

SET(PROJECT_FOLDER "boost_example")

ADD_SUBDIRECTORY(smart_pointers)
ADD_SUBDIRECTORY(ublas)
ADD_SUBDIRECTORY(uuid)

MESSAGE(STATUS "")
