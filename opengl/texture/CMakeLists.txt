#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(SDL2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR})

SET(HEADER_FILES
    texture_loader.h
    )

SET(SOURCE_FILES
    texture_loader.cpp
    )

ADD_LIBRARY(texture_loader STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET texture_loader PROPERTY FOLDER ${GLOBAL_PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(texture_loader ${SDL2_LIBRARY})

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
