#==============================================================================
# Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  INTRINSIC\n")

SET(GLOBAL_PROJECT_FOLDER "intrinsic")

ADD_SUBDIRECTORY(avx2_add)
ADD_SUBDIRECTORY(avx2_and)
ADD_SUBDIRECTORY(avx2_or)
ADD_SUBDIRECTORY(avx2_xor)
ADD_SUBDIRECTORY(avx2_max)
ADD_SUBDIRECTORY(avx2_min)
ADD_SUBDIRECTORY(avx512_add)
ADD_SUBDIRECTORY(avx512_and)
ADD_SUBDIRECTORY(avx512_or)
ADD_SUBDIRECTORY(avx512_xor)
ADD_SUBDIRECTORY(avx512_max)
ADD_SUBDIRECTORY(avx512_min)
ADD_SUBDIRECTORY(pause)
ADD_SUBDIRECTORY(rdtsc)
ADD_SUBDIRECTORY(rdtscp)
ADD_SUBDIRECTORY(rdrand32)

MESSAGE("")
