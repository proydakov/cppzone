#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#
#  ALUT_FOUND       - system has ALUT
#  ALUT_INCLUDE_DIR - the ALUT include directory
#  ALUT_LIBRARY     - Link these to use ALUT
#
#==============================================================================

IF(ALUT_INCLUDE_DIR AND ALUT_LIBRARY)

    SET(ALUT_FOUND TRUE)

ELSE()

    FIND_PATH(ALUT_INCLUDE_DIR AL/alut.h
        HINTS
        $ENV{ALUT_PATH}
        $ENV{PATH}
        PATH_SUFFIXES include
        PATHS
        /usr
        /usr/local)

    FIND_LIBRARY(ALUT_LIBRARY NAMES alut libalut
        HINTS
        $ENV{ALUT_PATH}
        $ENV{PATH}
        PATH_SUFFIXES lib lib64
        PATHS
        /usr
        /usr
        /usr/local)

    SET(ALUT_FOUND TRUE)

    IF(NOT ALUT_INCLUDE_DIR)
        SET(ALUT_FOUND FALSE)
    ENDIF(NOT ALUT_INCLUDE_DIR)

    IF(NOT ALUT_LIBRARY)
        SET(ALUT_FOUND FALSE)
    ENDIF(NOT ALUT_LIBRARY)
    
    MARK_AS_ADVANCED(ALUT_LIBRARY ALUT_INCLUDE_DIR)
    
ENDIF(ALUT_INCLUDE_DIR AND ALUT_LIBRARY)
