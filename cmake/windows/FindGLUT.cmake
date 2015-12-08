#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#
#  GLUT_FOUND       - system has GLUT
#  GLUT_INCLUDE_DIR - the GLUT include directory
#  GLUT_LIBRARY     - Link these to use GLUT
#
#==============================================================================

IF (WIN32)
    IF(GLUT_INCLUDE_DIR AND GLUT_LIBRARIES)
        SET(GLUT_FOUND TRUE)
    ELSE()
        FIND_PATH( GLUT_INCLUDE_DIR NAMES GL/glut.h
            PATHS
            ${GLUT_ROOT_PATH}/include
            )

        FIND_LIBRARY( GLUT_glutr_LIBRARY NAMES glut glut32 freeglut_static
            PATHS
            ${GLUT_ROOT_PATH}/lib
            )

        FIND_LIBRARY( GLUT_glutd_LIBRARY NAMES glut glut32 freeglut_static
            PATHS
            ${GLUT_ROOT_PATH}/lib
            )

        SET(GLUT_FOUND TRUE)
        IF(NOT GLUT_INCLUDE_DIR)
            SET(GLUT_FOUND FALSE)
        ENDIF(NOT GLUT_INCLUDE_DIR)

        IF(NOT GLUT_glutd_LIBRARY  OR NOT GLUT_glutr_LIBRARY)
            SET(GLUT_FOUND FALSE)
        ENDIF()

        SET(GLUT_LIBRARIES "debug" ${GLUT_glutd_LIBRARY} "optimized" ${GLUT_glutr_LIBRARY})
    ENDIF()
ENDIF()
