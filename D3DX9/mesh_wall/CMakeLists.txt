#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  mesh  wall  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.mesh.wall.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_mesh_wall.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

SET(TEXTURES_FILES
	textures/wall.png)

SET(CONFIG_FILES
	config_mesh_wall.h)

I_ADD_DIRECTX_EXECUTABLE(directx_mesh_wall ${SOURCE_FILES} ${TEXTURES_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx_mesh_wall ${DirectX_LIBRARIES})

SOURCE_GROUP(textures FILES ${TEXTURES_FILES})
