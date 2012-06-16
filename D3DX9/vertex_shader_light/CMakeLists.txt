#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  vertex  shader  light  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.vertex.shader.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_vertex_shader.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

SET(SHADER_FILES
    shaders/ambient.hlsl
    shaders/diffuse.hlsl
    shaders/specular.hlsl
    shaders/combo.hlsl)

SET(CONFIG_FILES
    config_directx_vertex_shader.h)

I_ADD_DIRECTX_EXECUTABLE(directx_shader_light ${SOURCE_FILES} ${SHADER_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx_shader_light ${DirectX_LIBRARIES})

SOURCE_GROUP(shaders FILES ${SHADER_FILES})