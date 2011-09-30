#=============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#=============================================================================
# Cmake for MAZE  PROJECT
#=============================================================================

MESSAGE(STATUS "CREATING  PROJECT  MAZE\n")

# add the include folder to the include path
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})

SET(HEADER_FILES
    maze_generator.h)

SET(SOURCE_FILES
    maze_generator.cpp)

SET(MAIN_FILES
    main.cpp)

# find OpenCV
FIND_PACKAGE(OpenCV)

IF(OpenCV_FOUND)
    MESSAGE(STATUS "OpenCV version:  ${OpenCV_VERSION}")
    INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
    LINK_DIRECTORIES(${OpenCV_LIB_DIR})
    ADD_LIBRARY(maze_generator ${HEADER_FILES} ${SOURCE_FILES})
    TARGET_LINK_LIBRARIES(maze_generator ${OpenCV_LIBS})

    ADD_EXECUTABLE(maze_viewer ${HEADER_FILES} ${MAIN_FILES})
    ADD_DEPENDENCIES(maze_viewer maze_generator)

    TARGET_LINK_LIBRARIES(maze_viewer ${OpenCV_LIBS})
    TARGET_LINK_LIBRARIES(maze_viewer maze_generator)
ELSE(OpenCV_FOUND)
    MESSAGE(ERROR "OpenCV NOT FOUND")
ENDIF(OpenCV_FOUND)
