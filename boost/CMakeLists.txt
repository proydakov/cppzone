#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  BOOST  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD BOOST EXAMPLE\n")

SET(GLOBAL_PROJECT_FOLDER "boost")

#ADD_SUBDIRECTORY(coro_1kk)
#ADD_SUBDIRECTORY(coro_threads)
ADD_SUBDIRECTORY(interrupt)
ADD_SUBDIRECTORY(graph)
ADD_SUBDIRECTORY(signals2)
#ADD_SUBDIRECTORY(random)
ADD_SUBDIRECTORY(uuid)

MESSAGE("")
