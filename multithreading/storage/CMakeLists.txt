#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(multithreading_storage ${SOURCE_FILES})
if(MSVC)
    target_compile_options(multithreading_storage PRIVATE "/wd4324")
endif()
