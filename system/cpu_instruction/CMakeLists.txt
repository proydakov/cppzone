#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(system_cpu_instruction ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(system_cpu_instruction PRIVATE "/wd5030")
endif()
