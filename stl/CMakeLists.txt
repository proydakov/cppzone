#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  STL\n")

SET(PROJECT_FOLDER "stl")

ADD_SUBDIRECTORY(str)
ADD_SUBDIRECTORY(vec)
ADD_SUBDIRECTORY(eq_range)
ADD_SUBDIRECTORY(heap)
ADD_SUBDIRECTORY(std_for_each)

MESSAGE("")
