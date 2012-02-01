#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  BINARY  TREE  EXAMPLE
#==============================================================================

MESSAGE(STATUS "create  binary  tree  data_structures")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(TREE_FILES
    binary_tree.h)

SET(MAIN_FILES 
    main.cpp)

ADD_EXECUTABLE(data_structures_binary_tree ${TREE_FILES} ${MAIN_FILES})
