#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  MULTITHREADING  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  MULTITHREADING\n")

SET(GLOBAL_PROJECT_FOLDER "multithreading")

IF(UNIX)
    ADD_SUBDIRECTORY(pthread)
    ADD_SUBDIRECTORY(semaphore)
ENDIF()

IF(CMAKE_SYSTEM_NAME MATCHES "^Linux")
    ADD_SUBDIRECTORY(mutex_impl)
ENDIF()

ADD_SUBDIRECTORY(cpu_load)
ADD_SUBDIRECTORY(storage)
ADD_SUBDIRECTORY(global_variable)
ADD_SUBDIRECTORY(global_shared)

MESSAGE("")
