#==============================================================================
# Copyright (c) 2015 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(boost_graph ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(boost_graph ${Boost_GRAPH_LIBRARY})
