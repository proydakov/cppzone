#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  simple  openal")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${OPENAL_SOUND_PATH})

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

CONFIGURE_FILE(config.simple.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_simple.h)

SET(CONFIG_FILES
    config_simple.h)

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(openal_simple ${SOURCE_FILES} ${CONFIG_FILES})
ADD_DEPENDENCIES(openal_simple openal_sound)
TARGET_LINK_LIBRARIES(openal_simple ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(openal_simple openal_sound)
