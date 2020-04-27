#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  EXAMPLE  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD ASM EXAMPLE\n")

SET(GLOBAL_PROJECT_FOLDER "asm")

IF(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    ADD_SUBDIRECTORY(amd64masm)
ENDIF()

#MESSAGE("")
