#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(boost_uuid ${SOURCE_FILES})
