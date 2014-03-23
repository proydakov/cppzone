#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYSTEM  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYSTEM\n")

SET(PROJECT_FOLDER "system")

ADD_SUBDIRECTORY(memory_leak)
ADD_SUBDIRECTORY(address_modification)

MESSAGE(STATUS "")
