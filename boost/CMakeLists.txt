#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  EXAMPLE  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD BOOST EXAMPLE\n")

SET(PROJECT_FOLDER "boost_example")

ADD_SUBDIRECTORY(uuid)
ADD_SUBDIRECTORY(random)
ADD_SUBDIRECTORY(async_tcp_server)

MESSAGE(STATUS "")
