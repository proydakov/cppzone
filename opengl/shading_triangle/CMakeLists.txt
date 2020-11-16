#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(opengl_shading_triangle ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_shading_triangle application)
