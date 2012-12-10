#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  pixel  shader  multitext  directx9")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.pixel.shader.multitext.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_pixel_shader_multitext.h)

FIND_PACKAGE(DIRECTX9)
INCLUDE_DIRECTORIES(${DIRECTX9_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

SET(SHADER_FILES
    shaders/multitexturing.hlsl
    shaders/vertex_transform.hlsl
    )

SET(TEXTURES_FILES
    textures/box.png
    textures/circle_light.png
    textures/multitexturing_text.png
    )

SET(CONFIG_FILES
    config_pixel_shader_multitext.h
    )

I_ADD_DIRECTX_EXECUTABLE(directx9_pixel_shader_multitext ${SOURCE_FILES} ${TEXTURES_FILES} ${SHADER_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx9_pixel_shader_multitext ${DIRECTX9_LIBRARIES})

SOURCE_GROUP(shaders  FILES ${SHADER_FILES})
SOURCE_GROUP(textures FILES ${TEXTURES_FILES})
