#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  D3DX9\n")

SET(PROJECT_FOLDER "directx")

SET(D3DX9_MODELS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/models)

ADD_SUBDIRECTORY(plane)
ADD_SUBDIRECTORY(alpha)
ADD_SUBDIRECTORY(light)
ADD_SUBDIRECTORY(objects)
ADD_SUBDIRECTORY(texture_cube)
ADD_SUBDIRECTORY(flat_mirror)
ADD_SUBDIRECTORY(flat_shadow)
ADD_SUBDIRECTORY(font)
ADD_SUBDIRECTORY(mesh_wall)
ADD_SUBDIRECTORY(mesh_text)
ADD_SUBDIRECTORY(mesh_from_x)
ADD_SUBDIRECTORY(vertex_shader_light)
ADD_SUBDIRECTORY(pixel_shader_multitext)
ADD_SUBDIRECTORY(terrain_height_map)

MESSAGE(STATUS "")
