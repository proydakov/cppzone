#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  ALGORITHMS  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  ALGORITHMS\n")

SET(PROJECT_FOLDER "algorithms")

ADD_SUBDIRECTORY(arrays)
ADD_SUBDIRECTORY(binary_tree)
ADD_SUBDIRECTORY(vector_vs_list)
ADD_SUBDIRECTORY(nums)
ADD_SUBDIRECTORY(permutation)
ADD_SUBDIRECTORY(sort)
ADD_SUBDIRECTORY(sort_memory)
ADD_SUBDIRECTORY(strings)

MESSAGE("")
