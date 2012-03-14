#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  STD  FOR  EACH  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  std  for  each  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(syntax_std_for_each ${SOURCE_FILES})
