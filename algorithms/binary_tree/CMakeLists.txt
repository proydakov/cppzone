#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(TREE_FILES
    binary_tree.h
    )

SET(MAIN_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(algorithms_binary_tree ${TREE_FILES} ${MAIN_FILES})
