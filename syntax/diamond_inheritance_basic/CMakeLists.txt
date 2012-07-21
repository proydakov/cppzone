#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  diamond  inheritance  basic  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${DIAMOND_INHERITANCE_PATH})

SET(HEADER_FILES
    ${DIAMOND_INHERITANCE_PATH}/diamond_inheritance.h)

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(syntax_diamond_inheritance_basic ${HEADER_FILES} ${SOURCE_FILES})
