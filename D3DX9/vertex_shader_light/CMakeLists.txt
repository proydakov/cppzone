#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.vertex.shader.light.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_vertex_shader_light.h)

FIND_PACKAGE(DIRECTX9)
INCLUDE_DIRECTORIES(${DIRECTX9_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

SET(SHADER_FILES
    shaders/ambient.hlsl
    shaders/diffuse.hlsl
    shaders/specular.hlsl
    shaders/combo.hlsl
    )

SET(CONFIG_FILES
    config_directx_vertex_shader_light.h
    )

I_ADD_DIRECTX_EXECUTABLE(directx9_vertex_shader_light ${SOURCE_FILES} ${SHADER_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx9_vertex_shader_light ${DIRECTX9_LIBRARIES})

SOURCE_GROUP(shaders FILES ${SHADER_FILES})