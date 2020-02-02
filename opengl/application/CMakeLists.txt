#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(SDL2)
FIND_PACKAGE(OpenGL)

SET(HEADER_FILES
    application.h
    )

SET(SOURCE_FILES
    application.cpp
    )

ADD_LIBRARY(application STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET application PROPERTY FOLDER ${GLOBAL_PROJECT_FOLDER})
TARGET_INCLUDE_DIRECTORIES(application PUBLIC SYSTEM ${SDL2_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(application PUBLIC ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
