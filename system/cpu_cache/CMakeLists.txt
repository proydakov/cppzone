#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(system_cpu_cache ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(system_cpu_cache PRIVATE "/wd4267")
endif()
