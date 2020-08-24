#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_rdrand64 ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_rdrand64 PRIVATE -mrdrnd)
endif()
