#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  flat  shadow  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(DirectX)
INCLUDE_DIRECTORIES(${DirectX_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_DIRECTX_EXECUTABLE(directx_flat_shadow ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(directx_flat_shadow ${DirectX_LIBRARIES})
