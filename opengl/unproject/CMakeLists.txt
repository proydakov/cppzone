#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(SYSTEM ${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(SYSTEM ${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(opengl_unproject ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_unproject ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_unproject ${GLUT_LIBRARIES})
