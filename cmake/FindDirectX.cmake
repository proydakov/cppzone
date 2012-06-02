#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#
#  DirectX_FOUND       - system has DirextX
#  DirectX_INCLUDE_DIR - the DirextX include directory
#  DirectX_LIBRARIES   - Link these to use DirextX
#
#==============================================================================

IF(WIN32)

    SET(DirectX_BASE_PATHS 
        "$ENV{DXSDK_DIR}"
        )
  
    FIND_PATH(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${DirectX_BASE_PATHS} PATH_SUFFIXES Include)
    
    SET(DirectX_LIB_PATH
        ${DirectX_BASE_PATHS}/Lib
    )
    
    SET(DirectX_PATH_SUFFIXES "x86")
    
    FIND_LIBRARY(DirectX_D9_RUNTIME_LIBRARY d3d9 HINTS ${DirectX_LIB_PATH} PATH_SUFFIXES ${DirectX_PATH_SUFFIXES})
    FIND_LIBRARY(DirectX_D3D9_LIBRARY_DEBUG d3dx9d HINTS ${DirectX_LIB_PATH} PATH_SUFFIXES ${DirectX_PATH_SUFFIXES})
    FIND_LIBRARY(DirectX_D3D9_LIBRARY_RELEASE d3dx9 d3d9 HINTS ${DirectX_LIB_PATH} PATH_SUFFIXES ${DirectX_PATH_SUFFIXES})
    
    SET(DirectX_LIBRARIES "optimized;" ${DirectX_D3D9_LIBRARY_RELEASE} ${DirectX_D9_RUNTIME_LIBRARY} 
                          "debug;" ${DirectX_D3D9_LIBRARY_DEBUG} ${DirectX_D9_RUNTIME_LIBRARY})
  
    SET(DirectX_FOUND TRUE)
  
    IF(NOT DirectX_INCLUDE_DIR)
        SET(DirectX_FOUND FALSE)
    ENDIF(NOT DirectX_INCLUDE_DIR)

    IF(NOT DirectX_LIBRARIES)
        SET(DirectX_FOUND FALSE)
    ENDIF(NOT DirectX_LIBRARIES)
    
    MARK_AS_ADVANCED(DirectX_INCLUDE_DIR 
        DirectX_LIBRARIES
        DirectX_D9_RUNTIME_LIBRARY
        DirectX_D3D9_LIBRARY_DEBUG 
        DirectX_D3D9_LIBRARY_RELEASE
    )
  
ELSE()

    SET(DirectX_FOUND FALSE)

ENDIF(WIN32)
