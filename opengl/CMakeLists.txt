#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENGL\n")

SET(GLOBAL_PROJECT_FOLDER "opengl")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(TEXTURE_LOADER_PIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(application)

ADD_SUBDIRECTORY(blending)
ADD_SUBDIRECTORY(get_system_info)

ADD_SUBDIRECTORY(texture)
ADD_SUBDIRECTORY(texture_cube)
#ADD_SUBDIRECTORY(follow_mouse)
ADD_SUBDIRECTORY(fill)
ADD_SUBDIRECTORY(shading_triangle)
ADD_SUBDIRECTORY(morphing)

IF(Boost_FOUND)
    ADD_SUBDIRECTORY(astar2d)
ENDIF()

MESSAGE("")
