#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${TEXTURE_LOADER_PIRECTORY})

CONFIGURE_FILE(config.terrain.height.map.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_terrain_height_map.h)

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(HEADER_FILES
    terrain.h
    )

SET(SOURCE_FILES
    terrain.cpp
    main.cpp
    )

SET(CONFIG_FILES
    config_terrain_height_map.h
    )

SET(IMAGE_FILES
    data/terrain.raw
    )

I_ADD_EXECUTABLE(opengl_terrain_height_map ${HEADER_FILES} ${SOURCE_FILES} ${CONFIG_FILES} ${IMAGE_FILES})
TARGET_LINK_LIBRARIES(opengl_terrain_height_map ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(opengl_terrain_height_map ${GLUT_LIBRARIES})

SOURCE_GROUP(images FILES ${IMAGE_FILES})
