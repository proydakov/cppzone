#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  MULTITHREADING  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  MULTITHREADING\n")

SET(PROJECT_FOLDER "multithreading")

IF(UNIX)
    ADD_SUBDIRECTORY(pthread)
ENDIF()

ADD_SUBDIRECTORY(global_variable)
ADD_SUBDIRECTORY(load_cpu)
ADD_SUBDIRECTORY(mutex_bad)

MESSAGE(STATUS "")
