#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================    

MESSAGE(STATUS "BUILD  OPENGL\n")

SET(PROJECT_FOLDER "opengl")

ADD_SUBDIRECTORY(opengl_get_system_info)
ADD_SUBDIRECTORY(opengl_hello_world)
ADD_SUBDIRECTORY(opengl_double_buffering)
ADD_SUBDIRECTORY(opengl_lines)
ADD_SUBDIRECTORY(opengl_follow_mouse)
ADD_SUBDIRECTORY(opengl_fill)
ADD_SUBDIRECTORY(opengl_cube)
ADD_SUBDIRECTORY(opengl_truncated_sphere)
ADD_SUBDIRECTORY(opengl_solar_system)
ADD_SUBDIRECTORY(opengl_robot_arm)
ADD_SUBDIRECTORY(opengl_unproject)
ADD_SUBDIRECTORY(opengl_shading_triangle)
ADD_SUBDIRECTORY(opengl_illuminated_sphere)
ADD_SUBDIRECTORY(opengl_movelight)
ADD_SUBDIRECTORY(opengl_color_material)
ADD_SUBDIRECTORY(opengl_blendeqn)
ADD_SUBDIRECTORY(opengl_alpha)
ADD_SUBDIRECTORY(opengl_alpha3d)
ADD_SUBDIRECTORY(opengl_aargb)
ADD_SUBDIRECTORY(opengl_multisamp)
ADD_SUBDIRECTORY(opengl_fog)
ADD_SUBDIRECTORY(opengl_fogcoord)
ADD_SUBDIRECTORY(opengl_display_list_torus)

MESSAGE(STATUS "")
