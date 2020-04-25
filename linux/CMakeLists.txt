#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  LINUX\n")

SET(GLOBAL_PROJECT_FOLDER "linux")

ADD_SUBDIRECTORY(avx2)
ADD_SUBDIRECTORY(fork_stdout)
ADD_SUBDIRECTORY(fork_memory)
ADD_SUBDIRECTORY(dynamic_code)
ADD_SUBDIRECTORY(dynamic_loading)
ADD_SUBDIRECTORY(udp_benchmark)
ADD_SUBDIRECTORY(hugepages)
ADD_SUBDIRECTORY(yield)
ADD_SUBDIRECTORY(interface_2_ip)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    ADD_SUBDIRECTORY(x64_stack)
endif()

MESSAGE("")
