#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  STL\n")

SET(GLOBAL_PROJECT_FOLDER "stl")

ADD_SUBDIRECTORY(str)
ADD_SUBDIRECTORY(vec)
ADD_SUBDIRECTORY(map)
ADD_SUBDIRECTORY(map2)
ADD_SUBDIRECTORY(map3)
ADD_SUBDIRECTORY(heap)
ADD_SUBDIRECTORY(eq_range)
ADD_SUBDIRECTORY(std_for_each)
ADD_SUBDIRECTORY(unordered_map)
ADD_SUBDIRECTORY(unordered_map2)

MESSAGE("")
