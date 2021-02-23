#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(SDL2)
FIND_PACKAGE(OpenGL)

SET(HEADER_FILES
    include/opengl_sdk/application.h
    )

SET(SOURCE_FILES
    src/application.cpp
    )

ADD_LIBRARY(opengl_sdk STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET opengl_sdk PROPERTY FOLDER ${GLOBAL_PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(opengl_sdk PUBLIC ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(opengl_sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
TARGET_INCLUDE_DIRECTORIES(opengl_sdk SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

IF(WIN32)
    TARGET_COMPILE_DEFINITIONS(opengl_sdk INTERFACE NOMINMAX)
ENDIF()
IF(APPLE)
    TARGET_COMPILE_OPTIONS(opengl_sdk INTERFACE -Wno-deprecated-declarations)
ENDIF()

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
