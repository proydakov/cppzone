#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  diamond  inheritance  casting  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${DIAMOND_INHERITANCE_PATH})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(syntax_diamond_inheritance_casting ${SOURCE_FILES})
