#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

I_ADD_EXECUTABLE(linux_udp_benchmark_client
    client.c
)

#==============================================================================

I_ADD_EXECUTABLE(linux_udp_benchmark_server
    server.c
)

