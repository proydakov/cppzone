#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(syntax_align ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(syntax_align PRIVATE "/wd4324")
endif()
