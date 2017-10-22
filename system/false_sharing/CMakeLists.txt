#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <e.proydakov@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(system_false_sharing ${SOURCE_FILES})
