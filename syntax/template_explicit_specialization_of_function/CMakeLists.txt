#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create template fun syntax")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(syntax_template_explicit_specialization_of_function ${SOURCE_FILES})
