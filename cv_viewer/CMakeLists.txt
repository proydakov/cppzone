#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for CV  VIEWER  PROJECT
#=============================================================================

MESSAGE(STATUS "CREATING  CV  VIEWER\n")

# add the include folder to the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

SET(HEADER_FILES
    cv_viewer.h)

SET(SOURCE_FILES
    cv_viewer.cpp)

# find OpenCV
FIND_PACKAGE(OpenCV)

IF(OpenCV_FOUND)
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    LINK_DIRECTORIES(${OpenCV_LIB_DIR})
    ADD_LIBRARY(cv_viewer ${HEADER_FILES} ${SOURCE_FILES})
    TARGET_LINK_LIBRARIES(cv_viewer ${OpenCV_LIBS})
ELSE(OpenCV_FOUND)
    MESSAGE(ERROR "OpenCV NOT FOUND")
ENDIF(OpenCV_FOUND)
