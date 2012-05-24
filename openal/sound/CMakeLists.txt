#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  sound  library  openal\n")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(OpenAL)
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})

SET(HEADER_FILES
    sound.h)

SET(SOURCE_FILES
    sound.cpp)

I_ADD_LIBRARY(openal_sound ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(openal_sound ${OPENAL_LIBRARY})
TARGET_LINK_LIBRARIES(openal_sound "libalut")
