#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  GLUT\n")

SET(PROJECT_FOLDER "graphics_glut")

ADD_SUBDIRECTORY(glut_game_mode)
ADD_SUBDIRECTORY(glut_cursor_mode)

MESSAGE(STATUS "")