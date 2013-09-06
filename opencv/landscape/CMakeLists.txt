#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create landscape opencv")

INCLUDE_DIRECTORIES(${CV_VIEWER})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    landscape_generator.h)

SET(SOURCE_FILES
    landscape_generator.cpp)

SET(MAIN_FILES
    main.cpp
    )

ADD_EXECUTABLE(opencv_maze_landscape ${HEADER_FILES} ${SOURCE_FILES} ${MAIN_FILES})
ADD_DEPENDENCIES(opencv_maze_landscape opencv_viewer)

TARGET_LINK_LIBRARIES(opencv_maze_landscape ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(opencv_maze_landscape opencv_viewer)
