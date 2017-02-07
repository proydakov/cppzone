#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  TASK\n")

SET(PROJECT_FOLDER "task")

ADD_SUBDIRECTORY(array_statistics)
ADD_SUBDIRECTORY(calculator)
ADD_SUBDIRECTORY(hanoi)
ADD_SUBDIRECTORY(lcd_display)
ADD_SUBDIRECTORY(big_file_sort)
ADD_SUBDIRECTORY(output_number_in_base)
ADD_SUBDIRECTORY(output_number_as_text)
ADD_SUBDIRECTORY(unique_ip)
ADD_SUBDIRECTORY(word_game)
ADD_SUBDIRECTORY(regex_string)

ADD_SUBDIRECTORY(zeptocode_rush_2014_a)
ADD_SUBDIRECTORY(zeptocode_rush_2014_b)

MESSAGE("")
