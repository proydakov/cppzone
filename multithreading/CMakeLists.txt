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

IF(UNIX AND NOT APPLE)
    ADD_SUBDIRECTORY(mutex_impl)
ENDIF()

ADD_SUBDIRECTORY(storage)
ADD_SUBDIRECTORY(global_variable)
ADD_SUBDIRECTORY(global_shared)
ADD_SUBDIRECTORY(load_cpu)

MESSAGE("")
