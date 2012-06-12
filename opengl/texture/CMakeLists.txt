#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  texture  loader  library")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(SDL)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})

SET(HEADER_FILES
    texture_loader.h)

SET(SOURCE_FILES
    texture_loader.cpp)

ADD_LIBRARY(texture_loader STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET texture_loader PROPERTY FOLDER ${PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(texture_loader ${SDL_LIBRARY})

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})