#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  texture  cube  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.texture.cube.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_texture_cube.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

SET(TEXTURES_FILES
	textures/box.png
	textures/directx.png
	textures/gamedev.png
	textures/goblet.png)

SET(CONFIG_FILES
	config_directx_texture_cube.h)

I_ADD_DIRECTX_EXECUTABLE(directx_texture_cube ${SOURCE_FILES} ${TEXTURES_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx_texture_cube ${DirectX_LIBRARIES})

SOURCE_GROUP(textures FILES ${TEXTURES_FILES})
