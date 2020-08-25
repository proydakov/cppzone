#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(multithreading_semaphore ${SOURCE_FILES})
IF(APPLE)
    TARGET_COMPILE_OPTIONS(multithreading_semaphore PUBLIC -Wno-deprecated-declarations)
ENDIF()
