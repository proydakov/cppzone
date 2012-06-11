#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  D3DX9\n")

SET(PROJECT_FOLDER "directx")

ADD_SUBDIRECTORY(plane)
ADD_SUBDIRECTORY(alpha)
ADD_SUBDIRECTORY(light)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(texture_cube)
ADD_SUBDIRECTORY(flat_mirror)
ADD_SUBDIRECTORY(flat_shadow)
ADD_SUBDIRECTORY(font)

MESSAGE(STATUS "")
