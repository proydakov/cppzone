#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  ASIO  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD ASIO EXAMPLE\n")

SET(GLOBAL_PROJECT_FOLDER "asio")

ADD_SUBDIRECTORY(async_tcp_server)
ADD_SUBDIRECTORY(async_speed_server)

MESSAGE("")
