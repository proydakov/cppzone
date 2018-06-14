#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENAL\n")

SET(GLOBAL_PROJECT_FOLDER "openal")

SET(OPENAL_SOUND_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sound)

ADD_SUBDIRECTORY(${OPENAL_SOUND_PATH})
ADD_SUBDIRECTORY(simple)

MESSAGE("")
