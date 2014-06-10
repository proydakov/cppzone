#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(file.reader.config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/file_reader_config.h)

SET(CONFIG_FILE
    file_reader_config.h
    )

SET(ADDITIONAL_FILES
    input.txt
    )

SET(HEADER_FILES
    text_file_reader.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(task_file_reader ${HEADER_FILES} ${SOURCE_FILES} ${ADDITIONAL_FILES} ${CONFIG_FILE})
