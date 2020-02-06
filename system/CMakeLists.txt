#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYSTEM  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYSTEM\n")

SET(GLOBAL_PROJECT_FOLDER "system")

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    ADD_SUBDIRECTORY(addr)
ENDIF()
ADD_SUBDIRECTORY(alloc)
ADD_SUBDIRECTORY(bigalloc)
ADD_SUBDIRECTORY(cpu_cache)
ADD_SUBDIRECTORY(cpu_freq)
ADD_SUBDIRECTORY(cpu_load)
ADD_SUBDIRECTORY(cpu_instruction)
ADD_SUBDIRECTORY(endian1)
ADD_SUBDIRECTORY(endian2)
ADD_SUBDIRECTORY(endian3)
ADD_SUBDIRECTORY(false_sharing)
ADD_SUBDIRECTORY(memory_leak)
ADD_SUBDIRECTORY(memory_bandwidth)
ADD_SUBDIRECTORY(str_storage)
ADD_SUBDIRECTORY(stack_size)
ADD_SUBDIRECTORY(stack_size_recursion)
ADD_SUBDIRECTORY(thread_stack_size)

MESSAGE("")
