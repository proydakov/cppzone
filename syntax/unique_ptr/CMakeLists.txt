#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(syntax_unique_ptr ${SOURCE_FILES})
IF(MSVC)
    TARGET_COMPILE_DEFINITIONS(syntax_unique_ptr PRIVATE _CRT_SECURE_NO_WARNINGS)
ENDIF()
