#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ENABLE_LANGUAGE(ASM_MASM)

SET(SOURCE_FILES
    main.cpp
    impl.asm
    )

I_ADD_EXECUTABLE(asm_amd64_asm_2_cpp ${SOURCE_FILES})
