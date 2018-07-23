#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CLIENT_SOURCE_FILES
    client.cpp
    )

SET(SERVER_SOURCE_FILES
    server.cpp
    )

I_ADD_EXECUTABLE(asio_sync_tcp_client ${CLIENT_SOURCE_FILES})
TARGET_LINK_LIBRARIES(asio_sync_tcp_client ${Boost_SYSTEM_LIBRARY})

I_ADD_EXECUTABLE(asio_async_tcp_server ${SERVER_SOURCE_FILES})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_THREAD_LIBRARY})
