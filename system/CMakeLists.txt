#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYSTEM  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYSTEM\n")

SET(PROJECT_FOLDER "system")

IF(UNIX)
	ADD_SUBDIRECTORY(pest)
ENDIF(UNIX)
ADD_SUBDIRECTORY(memory_leak)
