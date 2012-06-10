#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  flat  mirror  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config.directx.flat.mirror.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_directx_flat_mirror.h)

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(CONFIG_FILES
	config_directx_flat_mirror.h)

SET(SOURCE_FILES
    main.cpp)

I_ADD_DIRECTX_EXECUTABLE(directx_flat_mirror ${SOURCE_FILES} ${CONFIG_FILES})
TARGET_LINK_LIBRARIES(directx_flat_mirror ${DirectX_LIBRARIES})
