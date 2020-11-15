#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_avx2_add ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_avx2_add PRIVATE -mavx2)
endif()
