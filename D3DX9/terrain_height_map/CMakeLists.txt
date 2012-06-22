#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  terrain  height  map  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.terrain.height.map.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_terrain_height_map.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(HEADER_FILES
	mesh_generator.h
	geometry_loader.h)

SET(SOURCE_FILES
	mesh_generator.cpp
	geometry_loader.cpp
    main.cpp)

SET(EFFECTS_FILES
	effects/terrain.fx)

SET(TEXTURES_FILES
	textures/dirt.dds
	textures/stone.dds
	textures/grass.dds
	textures/blend_map.dds)

SET(CONFIG_FILES
    config_terrain_height_map.h)

I_ADD_DIRECTX_EXECUTABLE(directx_terrain_height_map ${HEADER_FILES} ${SOURCE_FILES} ${EFFECTS_FILES} ${TEXTURES_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx_terrain_height_map ${DirectX_LIBRARIES})

SOURCE_GROUP(textures FILES ${TEXTURES_FILES})
SOURCE_GROUP(effects FILES ${EFFECTS_FILES})