#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.mesh.from.x.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_mesh_from_x.h)

FIND_PACKAGE(DIRECTX9)
INCLUDE_DIRECTORIES(${DIRECTX9_INCLUDE_DIR})

SET(CONFIG_FILES
    config_directx_mesh_from_x.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_DIRECTX_EXECUTABLE(directx9_mesh_from_x ${SOURCE_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx9_mesh_from_x ${DIRECTX9_LIBRARIES})
