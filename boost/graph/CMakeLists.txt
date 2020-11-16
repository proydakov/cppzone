#==============================================================================
# Copyright (c) 2015 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(boost_graph ${SOURCE_FILES})
#TARGET_LINK_LIBRARIES(boost_graph ${Boost_GRAPH_LIBRARY})

if(MSVC)
    TARGET_COMPILE_OPTIONS(boost_graph PRIVATE "/wd4458")
    TARGET_COMPILE_OPTIONS(boost_graph PRIVATE "/wd4459")
    TARGET_COMPILE_OPTIONS(boost_graph PRIVATE "/wd26812")
endif()
