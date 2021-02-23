#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${TEXTURE_LOADER_PIRECTORY})

CONFIGURE_FILE(config.opengl.texture.cube.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_opengl_texture_cube.h)

SET(HEADER_FILES
    config_opengl_texture_cube.h
    )

SET(SOURCE_FILES
    main.cpp
    )

SET(IMAGE_FILES
    data/box.bmp
    data/opengl.bmp
    data/goblet.bmp
    data/gamedev.bmp
    )

I_ADD_EXECUTABLE(opengl_texture_cube ${HEADER_FILES} ${SOURCE_FILES} ${IMAGE_FILES})
ADD_DEPENDENCIES(opengl_texture_cube opengl_sdk)
ADD_DEPENDENCIES(opengl_texture_cube texture_loader)

TARGET_LINK_LIBRARIES(opengl_texture_cube opengl_sdk)
TARGET_LINK_LIBRARIES(opengl_texture_cube texture_loader)

SOURCE_GROUP(images FILES ${IMAGE_FILES})
