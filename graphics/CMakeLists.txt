#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  GRAPHICS  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  GRAPHICS\n")

FIND_PACKAGE(GLUT)

IF(GLUT_FOUND)
    ADD_SUBDIRECTORY(hello_opengl)
ENDIF(GLUT_FOUND)