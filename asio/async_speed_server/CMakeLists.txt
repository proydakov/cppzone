#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

I_ADD_EXECUTABLE(asio_speed_client
    client.cpp
)

TARGET_LINK_LIBRARIES(asio_speed_client ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(asio_speed_client ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(asio_speed_client ${Boost_SYSTEM_LIBRARY})

#==============================================================================

I_ADD_EXECUTABLE(asio_speed_server
    server.cpp
)

TARGET_LINK_LIBRARIES(asio_speed_server ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(asio_speed_server ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(asio_speed_server ${Boost_SYSTEM_LIBRARY})
