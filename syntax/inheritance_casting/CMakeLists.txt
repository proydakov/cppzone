#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  INHERITANCE  CASTING  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  inheritance  casting  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(syntax_inheritance_casting ${SOURCE_FILES})
