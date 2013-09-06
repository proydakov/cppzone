#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  OPENCV\n")

SET(PROJECT_FOLDER "opencv")

SET(CV_VIEWER ${CMAKE_CURRENT_SOURCE_DIR}/cv_viewer)

ADD_SUBDIRECTORY(cv_viewer)
ADD_SUBDIRECTORY(landscape)
ADD_SUBDIRECTORY(maze)
ADD_SUBDIRECTORY(videostab)

MESSAGE(STATUS "")
