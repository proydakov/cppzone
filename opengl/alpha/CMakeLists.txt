#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(opengl_alpha ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_alpha application)
