#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  astar2d  opengl")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS graph)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(HEADER_FILES
    maze.h
)

SET(SOURCE_FILES
    maze.cpp
    main.cpp
)

I_ADD_EXECUTABLE(opengl_astar2d ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_astar2d ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_astar2d ${GLUT_LIBRARIES})
