#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(DIRECTX9)
INCLUDE_DIRECTORIES(${DIRECTX9_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(directx9_plane ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(directx9_plane ${DIRECTX9_LIBRARIES})
