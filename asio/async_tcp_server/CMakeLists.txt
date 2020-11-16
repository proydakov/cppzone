#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

SET(CLIENT_SOURCE_FILES
    client.cpp
    )

SET(SERVER_SOURCE_FILES
    server.cpp
    )

I_ADD_EXECUTABLE(asio_sync_tcp_client ${CLIENT_SOURCE_FILES})
TARGET_LINK_LIBRARIES(asio_sync_tcp_client ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(asio_sync_tcp_client ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(asio_sync_tcp_client ${Boost_REGEX_LIBRARY})

I_ADD_EXECUTABLE(asio_async_tcp_server ${SERVER_SOURCE_FILES})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(asio_async_tcp_server ${Boost_CHRONO_LIBRARY})
