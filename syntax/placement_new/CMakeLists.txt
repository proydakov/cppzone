#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(syntax_placement_new ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(syntax_placement_new PRIVATE "/wd4324")
endif()
