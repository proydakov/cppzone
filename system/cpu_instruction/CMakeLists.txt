#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(system_cpu_instruction ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(system_cpu_instruction PRIVATE "/wd5030")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    TARGET_COMPILE_OPTIONS(system_cpu_instruction PRIVATE "-Wno-attributes" "-Wno-unknown-pragmas")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    TARGET_COMPILE_OPTIONS(system_cpu_instruction PRIVATE "-Wno-unknown-pragmas" "-Wno-unknown-attributes")
endif()
