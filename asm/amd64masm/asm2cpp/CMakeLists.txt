#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ENABLE_LANGUAGE(ASM)

SET(SOURCE_FILES
    main.cpp
    impl.asm
    )

set_source_files_properties(impl.asm PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

I_ADD_EXECUTABLE(asm_amd64masm_asm_2_cpp ${SOURCE_FILES})
