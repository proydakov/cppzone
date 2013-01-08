#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  OPENGLES\n")

SET(PROJECT_FOLDER "opengles")

FIND_LIBRARY(EGL_LIBRARY
    NAMES EGL
    )

FIND_LIBRARY(GLESV2_LIBRARY
    NAMES GLESv2
    )

FIND_PACKAGE(Qt4 COMPONENTS QtGui QtCore)

IF(EGL_LIBRARY AND GLESV2_LIBRARY AND QT4_FOUND)
    ADD_SUBDIRECTORY(hidden_rendering)
ENDIF()

MESSAGE(STATUS "")
