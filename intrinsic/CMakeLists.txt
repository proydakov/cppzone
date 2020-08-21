#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  INTRINSIC\n")

SET(GLOBAL_PROJECT_FOLDER "intrinsic")

ADD_SUBDIRECTORY(avx2)
ADD_SUBDIRECTORY(avx512)
ADD_SUBDIRECTORY(rdtsc)
ADD_SUBDIRECTORY(rdtscp)

MESSAGE("")
