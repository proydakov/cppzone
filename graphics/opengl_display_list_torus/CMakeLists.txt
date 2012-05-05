#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  opengl  display  list  torus  graphics")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(graphics_opengl_display_list_torus ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(graphics_opengl_display_list_torus ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(graphics_opengl_display_list_torus ${GLUT_LIBRARIES})

