#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  ADRESS  BASE  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  adress  base  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(syntax_adress_base ${SOURCE_FILES})
