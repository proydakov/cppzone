#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create asio tcp server boost")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

SET(CLIENT_SOURCE_FILES
    client.cpp
    )

SET(SERVER_SOURCE_FILES
    server.cpp
    )

I_ADD_EXECUTABLE(boost_sync_tcp_client ${CLIENT_SOURCE_FILES})
TARGET_LINK_LIBRARIES(boost_sync_tcp_client ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(boost_sync_tcp_client ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(boost_sync_tcp_client ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(boost_sync_tcp_client ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(boost_sync_tcp_client ${Boost_CHRONO_LIBRARY})

I_ADD_EXECUTABLE(boost_async_tcp_server ${SERVER_SOURCE_FILES})
TARGET_LINK_LIBRARIES(boost_async_tcp_server ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(boost_async_tcp_server ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(boost_async_tcp_server ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(boost_async_tcp_server ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(boost_async_tcp_server ${Boost_CHRONO_LIBRARY})
