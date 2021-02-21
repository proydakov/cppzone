#==============================================================================
# Copyright (c) 2021 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENGLES_V2\n")

SET(GLOBAL_PROJECT_FOLDER "opengles2")

SET(TEXTURE_LOADER_PIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(opengles2_sdk)

ADD_SUBDIRECTORY(stencil)
ADD_SUBDIRECTORY(culling)
ADD_SUBDIRECTORY(triangle)
ADD_SUBDIRECTORY(gradient)
ADD_SUBDIRECTORY(texture)
ADD_SUBDIRECTORY(tga_viewer)
ADD_SUBDIRECTORY(multitexture)

MESSAGE("")
