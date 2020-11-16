#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(syntax_alignas ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(syntax_alignas PRIVATE "/wd4324")
endif()
