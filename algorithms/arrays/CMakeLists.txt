#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    iarray.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(algorithms_arrays ${HEADER_FILES} ${SOURCE_FILES})
