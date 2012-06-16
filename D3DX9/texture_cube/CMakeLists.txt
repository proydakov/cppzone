#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  texture_cube  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.texture.cube.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_texture_cube.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

SET(CONFIG_FILES
	config_directx_texture_cube.h)

I_ADD_DIRECTX_EXECUTABLE(directx_texture_cube ${SOURCE_FILES} ${CONFIG_FILES} ${IMAGE_FILES})
TARGET_LINK_LIBRARIES(directx_texture_cube ${DirectX_LIBRARIES})