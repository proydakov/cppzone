#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYSTEM  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYSTEM\n")

SET(PROJECT_FOLDER "system")

ADD_SUBDIRECTORY(alloc)
ADD_SUBDIRECTORY(bigalloc)
ADD_SUBDIRECTORY(cpu_cache)
ADD_SUBDIRECTORY(cpu_freq)
ADD_SUBDIRECTORY(endian)
ADD_SUBDIRECTORY(endian2)
ADD_SUBDIRECTORY(endian3)
ADD_SUBDIRECTORY(false_sharing)
ADD_SUBDIRECTORY(cpu_instruction)
ADD_SUBDIRECTORY(memory_leak)
ADD_SUBDIRECTORY(stack_size)
ADD_SUBDIRECTORY(stack_size_recursion)
ADD_SUBDIRECTORY(thread_stack_size)

MESSAGE("")
