#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  template  fun  syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

ADD_EXECUTABLE(syntax_template_fun ${SOURCE_FILES})
