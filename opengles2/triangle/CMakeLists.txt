#==============================================================================
# Copyright (c) 2021 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

CONFIGURE_FILE(resource.h.tmpl resource.h)
I_ADD_EXECUTABLE(opengles2_triangle ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengles2_triangle opengles2_sdk)
