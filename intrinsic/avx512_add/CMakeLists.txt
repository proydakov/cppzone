#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_avx512_add ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_avx512_add PRIVATE -mavx512f)
endif()
