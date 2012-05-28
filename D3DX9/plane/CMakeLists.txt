#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  plane  directx")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Directx)
INCLUDE_DIRECTORIES(${Directx_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(directx_plane ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(directx_plane ${DirectX_LIBRARIES})
