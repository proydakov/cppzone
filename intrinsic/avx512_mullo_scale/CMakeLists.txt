#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_avx512_mullo_scale ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_avx512_mullo_scale PRIVATE -mavx512f)
endif()
