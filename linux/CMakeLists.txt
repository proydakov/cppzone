#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  LINUX\n")

SET(PROJECT_FOLDER "linux")

ADD_SUBDIRECTORY(fork_stdout)
ADD_SUBDIRECTORY(fork_memory)
ADD_SUBDIRECTORY(pest)
ADD_SUBDIRECTORY(dynamic_code)
ADD_SUBDIRECTORY(dynamic_loading)
ADD_SUBDIRECTORY(hugepages)
ADD_SUBDIRECTORY(system_stack)

MESSAGE("")
