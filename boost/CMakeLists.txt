#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  EXAMPLE  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD BOOST EXAMPLE\n")

SET(PROJECT_FOLDER "boost")

ADD_SUBDIRECTORY(async_tcp_server)
ADD_SUBDIRECTORY(interrupt)
ADD_SUBDIRECTORY(signals2)
ADD_SUBDIRECTORY(random)
ADD_SUBDIRECTORY(uuid)

MESSAGE("")
