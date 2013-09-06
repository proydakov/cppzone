#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create viewer opencv")

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    cv_viewer.h
    )

SET(SOURCE_FILES
    cv_viewer.cpp
    )

ADD_LIBRARY(opencv_viewer ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET opencv_viewer PROPERTY FOLDER ${PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(opencv_viewer ${OpenCV_LIBS})
