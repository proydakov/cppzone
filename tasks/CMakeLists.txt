#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  TASKS  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  TASKS\n")

SET(PROJECT_FOLDER "tasks")

ADD_SUBDIRECTORY(distance_on_the_tree)
ADD_SUBDIRECTORY(calculator)
ADD_SUBDIRECTORY(file_reader)
ADD_SUBDIRECTORY(hanoi)
ADD_SUBDIRECTORY(output_number_in_base)
ADD_SUBDIRECTORY(output_number_as_text)
ADD_SUBDIRECTORY(t9)

MESSAGE(STATUS "")
