#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(OpenAL)
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})

FIND_PACKAGE(ALUT)
INCLUDE_DIRECTORIES(${ALUT_INCLUDE_DIR})

SET(HEADER_FILES
    sound.h)

SET(SOURCE_FILES
    sound.cpp)

ADD_LIBRARY(openal_sound STATIC ${FILES} ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET openal_sound PROPERTY FOLDER ${PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(openal_sound ${OPENAL_LIBRARY})
TARGET_LINK_LIBRARIES(openal_sound ${ALUT_LIBRARY})
