#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  GRAPHICS  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  GRAPHICS\n")

SET(PROJECT_FOLDER "graphics")

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)

IF(GLUT_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY(opengl_hello_world)
    ADD_SUBDIRECTORY(opengl_double_buffering)
ENDIF(GLUT_FOUND AND OPENGL_FOUND)
