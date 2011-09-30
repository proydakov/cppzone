#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for LANDSCAPE  PROJECT
#=============================================================================

MESSAGE(STATUS "CREATING  PROJECT  LANDSCAPE\n")

# add the include folder to the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

SET(HEADER_FILES
    landscape_generator.h)

SET(SOURCE_FILES
    landscape_generator.cpp)

SET(MAIN_FILES
    main.cpp)

# find OpenCV
FIND_PACKAGE(OpenCV)

IF(OpenCV_FOUND)
    MESSAGE(STATUS "OpenCV version:  ${OpenCV_VERSION}")
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    LINK_DIRECTORIES(${OpenCV_LIB_DIR})
    ADD_LIBRARY(landscape_generator ${HEADER_FILES} ${SOURCE_FILES})
    TARGET_LINK_LIBRARIES(landscape_generator ${OpenCV_LIBS})

    ADD_EXECUTABLE(landscape_viewer ${HEADER_FILES} ${MAIN_FILES})
    ADD_DEPENDENCIES(landscape_viewer landscape_generator)

    TARGET_LINK_LIBRARIES(landscape_viewer ${OpenCV_LIBS})
    TARGET_LINK_LIBRARIES(landscape_viewer landscape_generator)
ELSE(OpenCV_FOUND)
    MESSAGE(ERROR "OpenCV NOT FOUND")
ENDIF(OpenCV_FOUND)
