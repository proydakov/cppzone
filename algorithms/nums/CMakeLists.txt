#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(HEADER_FILES
    operations_with_numbers.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(algorithms_nums ${HEADER_FILES} ${SOURCE_FILES})
