#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  opengl  solar  system  graphics")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(HEADER_FILES
	solar_system_info.h)

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(graphics_opengl_solar_system ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(graphics_opengl_solar_system ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(graphics_opengl_solar_system ${GLUT_LIBRARIES})

