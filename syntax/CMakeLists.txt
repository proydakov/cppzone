#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYNTAX  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYNTAX\n")

SET(DIAMOND_INHERITANCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/diamond_inheritance)

ADD_SUBDIRECTORY(adress_base)
ADD_SUBDIRECTORY(diamond_inheritance_basic)
ADD_SUBDIRECTORY(diamond_inheritance_casting)
ADD_SUBDIRECTORY(exception_basic)
ADD_SUBDIRECTORY(inheritance_basic)
ADD_SUBDIRECTORY(inheritance_casting)
ADD_SUBDIRECTORY(std_for_each)
ADD_SUBDIRECTORY(template_fun)
