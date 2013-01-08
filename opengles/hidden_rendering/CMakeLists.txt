#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  hidden  rendering  opengles")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

INCLUDE(${QT_USE_FILE})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(opengles_hidden_rendering ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengles_hidden_rendering ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(opengles_hidden_rendering ${GLESV2_LIBRARY})
TARGET_LINK_LIBRARIES(opengles_hidden_rendering ${EGL_LIBRARY})
TARGET_LINK_LIBRARIES(opengles_hidden_rendering benchmark)
