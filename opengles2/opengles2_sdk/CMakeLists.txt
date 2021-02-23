#==============================================================================
# Copyright (c) 2021 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(SDL2)
IF(NOT APPLE)
    FIND_LIBRARY(GLESv2_LIBRARY GLESv2)
ELSE()
    FIND_PACKAGE(OpenGL REQUIRED)
ENDIF()

SET(HEADER_FILES
    include/opengles2_sdk/opengles2_application.h
    include/opengles2_sdk/opengles2_math.h
    include/opengles2_sdk/opengles2_mesh.h
    include/opengles2_sdk/opengles2_shader.h
    include/opengles2_sdk/opengles2_program.h
    include/opengles2_sdk/opengles2_texture.h
    )

SET(SOURCE_FILES
    src/opengles2_math.cpp
    src/opengles2_mesh.cpp
    src/opengles2_shader.cpp
    src/opengles2_program.cpp
    src/opengles2_texture.cpp
    src/opengles2_application.cpp
    )

ADD_LIBRARY(opengles2_sdk STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET opengles2_sdk PROPERTY FOLDER ${GLOBAL_PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(opengles2_sdk PUBLIC ${SDL2_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(opengles2_sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
TARGET_INCLUDE_DIRECTORIES(opengles2_sdk SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

IF(NOT APPLE)
    TARGET_LINK_LIBRARIES(opengles2_sdk PUBLIC ${GLESv2_LIBRARY})
ELSE()
    TARGET_LINK_LIBRARIES(opengles2_sdk PUBLIC ${OPENGL_LIBRARIES})
    TARGET_COMPILE_OPTIONS(opengles2_sdk PUBLIC -Wno-deprecated-declarations)
ENDIF()

IF(WIN32)
    TARGET_COMPILE_DEFINITIONS(opengles2_sdk PUBLIC NOMINMAX)
ENDIF()

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
