#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  identification  number  task")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(identification.number.config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/identification_number_config.h)

SET(CONFIG_FILE
    identification_number_config.h)

SET(ADDITIONAL_FILES
    1.txt
    2.txt
    3.txt
    4.txt
    5.txt
    6.txt
    7.txt
    8.txt
    9.txt)

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(task_identification_number ${CONFIG_FILE} ${ADDITIONAL_FILES} ${SOURCE_FILES})
