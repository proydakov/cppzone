#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(syntax_sizeof ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(syntax_sizeof PRIVATE "/wd4324")
endif()
