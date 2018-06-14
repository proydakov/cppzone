#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#
#  GLUT_FOUND       - system has GLUT
#  GLUT_INCLUDE_DIR - the GLUT include directory
#  GLUT_LIBRARIES   - Link these to use GLUT
#  GLUT_BINARIES    - FREEGLUT DLL
#
#==============================================================================

IF(GLUT_INCLUDE_DIR AND GLUT_LIBRARIES AND GLUT_BINARIES)

    SET(GLUT_FOUND TRUE)

ELSE()

    FIND_PATH(GLUT_INCLUDE_DIR GL/glut.h
        HINTS
        PATH_SUFFIXES include
        PATHS
        ${PROJECT_SOURCE_DIR}/thirdparty/windows/freeglut/include
	)

    FIND_LIBRARY(GLUT_LIBRARIES NAMES freeglut
        HINTS
        PATHS
        ${PROJECT_SOURCE_DIR}/thirdparty/windows/freeglut/lib/${GLOBAL_SEARCH_SUFFIX}
	)

    SET(GLUT_FOUND TRUE)
    IF(NOT GLUT_INCLUDE_DIR)
        SET(GLUT_FOUND FALSE)
    ENDIF(NOT GLUT_INCLUDE_DIR)

    IF(NOT GLUT_LIBRARIES)
        SET(GLUT_FOUND FALSE)
    ENDIF(NOT GLUT_LIBRARIES)

    MARK_AS_ADVANCED(GLUT_LIBRARIES GLUT_INCLUDE_DIR GLUT_BINARIES)

ENDIF()
