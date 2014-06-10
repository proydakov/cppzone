#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  TASKS  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  TASKS\n")

SET(PROJECT_FOLDER "tasks")

ADD_SUBDIRECTORY(array_statistics)
ADD_SUBDIRECTORY(calculator)
ADD_SUBDIRECTORY(file_reader)
ADD_SUBDIRECTORY(hanoi)
ADD_SUBDIRECTORY(lcd_display)
ADD_SUBDIRECTORY(read_big_file)
ADD_SUBDIRECTORY(invert_list)
ADD_SUBDIRECTORY(output_number_in_base)
ADD_SUBDIRECTORY(output_number_as_text)
ADD_SUBDIRECTORY(t9)
ADD_SUBDIRECTORY(unique_ip)

MESSAGE(STATUS "")
