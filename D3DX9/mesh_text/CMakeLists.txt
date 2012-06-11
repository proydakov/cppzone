#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  mesh  text  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_DIRECTX_EXECUTABLE(directx_mesh_text ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(directx_mesh_text ${DirectX_LIBRARIES})
