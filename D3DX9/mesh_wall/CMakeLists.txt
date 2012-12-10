#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  mesh  wall  directx9")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.mesh.wall.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_mesh_wall.h)

FIND_PACKAGE(DIRECTX9)
INCLUDE_DIRECTORIES(${DIRECTX9_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

SET(TEXTURES_FILES
    textures/wall.png
    )

SET(CONFIG_FILES
    config_mesh_wall.h
    )

I_ADD_DIRECTX_EXECUTABLE(directx9_mesh_wall ${SOURCE_FILES} ${TEXTURES_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx9_mesh_wall ${DIRECTX9_LIBRARIES})

SOURCE_GROUP(textures FILES ${TEXTURES_FILES})
