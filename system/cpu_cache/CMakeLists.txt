#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(system_cpu_cache ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(system_cpu_cache PRIVATE "/wd4267")
endif()
