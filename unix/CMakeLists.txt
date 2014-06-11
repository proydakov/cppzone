#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  UNIX  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  UNIX\n")

SET(PROJECT_FOLDER "unix")

ADD_SUBDIRECTORY(fork_stdout)
ADD_SUBDIRECTORY(fork_memory)
ADD_SUBDIRECTORY(pest)
ADD_SUBDIRECTORY(dynamic_loading)

MESSAGE("")
