#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENGL\n")

SET(GLOBAL_PROJECT_FOLDER "opengl")

SET(TEXTURE_LOADER_PIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(opengl_sdk)

ADD_SUBDIRECTORY(blending)
ADD_SUBDIRECTORY(get_system_info)

ADD_SUBDIRECTORY(texture)
ADD_SUBDIRECTORY(texture_cube)
ADD_SUBDIRECTORY(fill)
ADD_SUBDIRECTORY(shading_triangle)
ADD_SUBDIRECTORY(morphing)

IF(Boost_FOUND)
    ADD_SUBDIRECTORY(astar2d)
ENDIF()

MESSAGE("")
