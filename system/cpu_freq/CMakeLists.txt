#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <e.proydakov@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(system_cpu_freq ${SOURCE_FILES})
if(MSVC)
    TARGET_COMPILE_OPTIONS(system_cpu_freq PRIVATE "/wd5030")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    TARGET_COMPILE_OPTIONS(system_cpu_freq PRIVATE "-Wno-attributes" "-Wno-unknown-pragmas")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    TARGET_COMPILE_OPTIONS(system_cpu_freq PRIVATE "-Wno-unknown-pragmas" "-Wno-unknown-attributes")
endif()
