#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(intrinsic_avx512_xor ${SOURCE_FILES})
if(UNIX)
    target_compile_options(intrinsic_avx512_xor PRIVATE -mavx512f)
endif()
