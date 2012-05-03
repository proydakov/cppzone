#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  GRAPHICS  PROJECT
#==============================================================================

MESSAGE(STATUS "\nBUILD  GRAPHICS\n")

SET(PROJECT_FOLDER "graphics")

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)

IF(GLUT_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY(glut_game_mode)
    ADD_SUBDIRECTORY(glut_cursor_mode)    
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
ENDIF(GLUT_FOUND AND OPENGL_FOUND)