#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  dynamic  loading  unix")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_LIBRARY(DL_LIBRARY
    NAMES dl
    )

SET(LIBRARY_FILES
    object.h
    object.cpp
    )

ADD_LIBRARY(unix_object ${LIBRARY_FILES})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(unix_dynamic_loading ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(unix_dynamic_loading ${DL_LIBRARY})
