#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(SDL2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(opengl_alpha ${SOURCE_FILES})
ADD_DEPENDENCIES(opengl_alpha application)

TARGET_LINK_LIBRARIES(opengl_alpha ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_alpha application)
