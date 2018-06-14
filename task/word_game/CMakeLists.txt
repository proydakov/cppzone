#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

CONFIGURE_FILE(config_path.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_path.h)

SET(DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/data)
FILE(GLOB_RECURSE DATA_FILES ${DATA_PATH}/*)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/data/en_alphabet.txt ${CMAKE_CURRENT_BINARY_DIR}/en_alphabet.txt)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/data/ru_alphabet.txt ${CMAKE_CURRENT_BINARY_DIR}/ru_alphabet.txt)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/data/en_alphabet.txt ${EXECUTABLE_OUTPUT_PATH}/en_alphabet.txt)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/data/ru_alphabet.txt ${EXECUTABLE_OUTPUT_PATH}/ru_alphabet.txt)

SET(HEADER_FILES
    node.h
    common.h
    data_reader.h
    data_validator.h
    word_game_solver.h
    word_game_app.h
    )

SET(SOURCE_FILES
    data_reader.cpp
    data_validator.cpp
    word_game_solver.cpp
    word_game_app.cpp
    )

I_ADD_LIBRARY(task_word_game_impl
    ${HEADER_FILES}
    ${SOURCE_FILES}
    )

I_ADD_EXECUTABLE(task_word_game
    main.cpp
    )

ADD_DEPENDENCIES(task_word_game task_word_game_impl)
TARGET_LINK_LIBRARIES(task_word_game task_word_game_impl)

SOURCE_GROUP("data" FILES
    ${DATA_FILES}
    )

#==============================================================================

I_ADD_EXECUTABLE(task_word_game_test_data_reader
    config_path.h
    test_data_reader.cpp
    )

ADD_DEPENDENCIES(task_word_game_test_data_reader task_word_game_impl)
TARGET_LINK_LIBRARIES(task_word_game_test_data_reader task_word_game_impl)

#==============================================================================

I_ADD_EXECUTABLE(task_word_game_test_data_validator
    test_data_validator.cpp
    )

ADD_DEPENDENCIES(task_word_game_test_data_validator task_word_game_impl)
TARGET_LINK_LIBRARIES(task_word_game_test_data_validator task_word_game_impl)

#==============================================================================

I_ADD_EXECUTABLE(task_word_game_test_app
    config_path.h
    test_alg.cpp
	${DATA_FILES}
    )

ADD_DEPENDENCIES(task_word_game_test_app task_word_game_impl)
TARGET_LINK_LIBRARIES(task_word_game_test_app task_word_game_impl)
