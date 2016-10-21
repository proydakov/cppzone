#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYSTEM  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYSTEM\n")

SET(PROJECT_FOLDER "system")

ADD_SUBDIRECTORY(cpu_cache)
ADD_SUBDIRECTORY(memory_leak)
ADD_SUBDIRECTORY(stack_size)
ADD_SUBDIRECTORY(stack_size_recursion)

MESSAGE("")
