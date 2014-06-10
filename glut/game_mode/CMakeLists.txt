#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(glut_game_mode ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(glut_game_mode ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(glut_game_mode ${GLUT_LIBRARIES})
