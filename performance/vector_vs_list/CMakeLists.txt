#==============================================================================
# Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create vector_vs_list performance")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(performance_vector_vs_list ${SOURCE_FILES})
ADD_DEPENDENCIES(performance_vector_vs_list benchmark)
TARGET_LINK_LIBRARIES(performance_vector_vs_list benchmark)
