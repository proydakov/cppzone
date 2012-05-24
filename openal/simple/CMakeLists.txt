#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  simple  openal")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${OPENAL_SOUND_PATH})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(openal_simple ${SOURCE_FILES})
ADD_DEPENDENCIES(openal_simple openal_sound)
TARGET_LINK_LIBRARIES(openal_simple openal_sound)
