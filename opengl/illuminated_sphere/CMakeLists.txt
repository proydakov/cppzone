#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  illuminated  sphere  opengl")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(opengl_illuminated_sphere ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_illuminated_sphere ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_illuminated_sphere ${GLUT_LIBRARIES})
