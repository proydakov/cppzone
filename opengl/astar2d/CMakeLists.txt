#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create astar2d opengl")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS graph)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(SYSTEM ${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(SDL2)
INCLUDE_DIRECTORIES(SYSTEM ${SDL2_INCLUDE_DIR})

SET(HEADER_FILES
    maze.h
)

SET(SOURCE_FILES
    maze.cpp
    main.cpp
)

I_ADD_EXECUTABLE(opengl_astar2d ${HEADER_FILES} ${SOURCE_FILES})
ADD_DEPENDENCIES(opengl_astar2d application)

TARGET_LINK_LIBRARIES(opengl_astar2d ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_astar2d application)
