#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create vector_vs_list data_structures")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(data_structures_vector_vs_list ${SOURCE_FILES})
