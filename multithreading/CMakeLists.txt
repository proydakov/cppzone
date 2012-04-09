#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  MULTITHREADING  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  MULTITHREADING\n")

ADD_SUBDIRECTORY(global_variable)
ADD_SUBDIRECTORY(load_cpu)
IF(UNIX)
	ADD_SUBDIRECTORY(pthread)
ENDIF(UNIX)
