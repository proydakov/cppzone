#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(nt_divide_by_zero ${SOURCE_FILES})
set_target_properties(nt_divide_by_zero PROPERTIES COMPILE_FLAGS "/EHa")
