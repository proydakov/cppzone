#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENGL\n")

SET(PROJECT_FOLDER "opengl")

ADD_SUBDIRECTORY(get_system_info)
ADD_SUBDIRECTORY(hello_world)
ADD_SUBDIRECTORY(double_buffering)
ADD_SUBDIRECTORY(lines)
ADD_SUBDIRECTORY(follow_mouse)
ADD_SUBDIRECTORY(fill)
ADD_SUBDIRECTORY(cube)
ADD_SUBDIRECTORY(truncated_sphere)
ADD_SUBDIRECTORY(solar_system)
ADD_SUBDIRECTORY(robot_arm)
ADD_SUBDIRECTORY(unproject)
ADD_SUBDIRECTORY(shading_triangle)
ADD_SUBDIRECTORY(illuminated_sphere)
ADD_SUBDIRECTORY(movelight)
ADD_SUBDIRECTORY(color_material)
ADD_SUBDIRECTORY(blendeqn)
ADD_SUBDIRECTORY(alpha)
ADD_SUBDIRECTORY(alpha3d)
ADD_SUBDIRECTORY(aargb)
ADD_SUBDIRECTORY(multisamp)
ADD_SUBDIRECTORY(fog)
ADD_SUBDIRECTORY(fogcoord)
ADD_SUBDIRECTORY(display_list_torus)
ADD_SUBDIRECTORY(morphing)
ADD_SUBDIRECTORY(terrain_height_map)

FIND_PACKAGE(SDL)
IF(SDL_FOUND)
    SET(TEXTURE_LOADER_PIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    ADD_SUBDIRECTORY(texture)
    ADD_SUBDIRECTORY(texture_cube)
ELSE()
    MESSAGE(STATUS "SDL NOT FOUND !!! Texture loader is not created.")
ENDIF(SDL_FOUND)

MESSAGE(STATUS "")
