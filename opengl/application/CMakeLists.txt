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
TARGET_LINK_LIBRARIES(application PUBLIC ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(application SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

IF(WIN32)
    TARGET_COMPILE_DEFINITIONS(application INTERFACE NOMINMAX)
ENDIF()
IF(APPLE)
    TARGET_COMPILE_OPTIONS(application INTERFACE -Wno-deprecated-declarations)
ENDIF()

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
