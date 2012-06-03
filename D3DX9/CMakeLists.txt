#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  DIRECTX\n")

SET(PROJECT_FOLDER "directx")

ADD_SUBDIRECTORY(plane)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(light)
ADD_SUBDIRECTORY(texture_cube)

MESSAGE(STATUS "")
