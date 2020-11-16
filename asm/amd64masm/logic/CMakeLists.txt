#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

ENABLE_LANGUAGE(ASM_MASM)

SET(SOURCE_FILES
    main.cpp
    impl.asm
    )

I_ADD_EXECUTABLE(asm_amd64masm_logic ${SOURCE_FILES})
