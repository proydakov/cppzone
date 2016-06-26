#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(boost_coro_threads ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(boost_coro_threads ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(boost_coro_threads ${Boost_CONTEXT_LIBRARY})
TARGET_LINK_LIBRARIES(boost_coro_threads ${Boost_COROUTINE_LIBRARY})
