#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for LANDSCAPE  PROJECT
#==============================================================================

MESSAGE(STATUS "CREATING  LANDSCAPE  GENERATOR\n")

# add the include folder to the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CV_VIEWER})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

SET(HEADER_FILES
    landscape_generator.h)

SET(SOURCE_FILES
    landscape_generator.cpp)

SET(MAIN_FILES
    main.cpp)

# find OpenCV
FIND_PACKAGE(OpenCV)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIB_DIR})
ADD_EXECUTABLE(landscape_viewer ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILES})
ADD_DEPENDENCIES(landscape_viewer cv_viewer)

TARGET_LINK_LIBRARIES(landscape_viewer ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(landscape_viewer cv_viewer)
