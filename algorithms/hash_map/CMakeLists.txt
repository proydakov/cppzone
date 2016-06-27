#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    hash_map.h
    hash_functor.h
    equal_functor.h
    )

SET(SOURCE_FILES
   main.cpp
   )

I_ADD_EXECUTABLE(algorithms_hash_map ${HEADER_FILES} ${SOURCE_FILES})
