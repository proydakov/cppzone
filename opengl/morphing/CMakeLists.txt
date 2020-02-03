#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(HEADER_FILES
    cube.h
    chaos.h
    torus.h
    lines.h
    sphere.h
    spiral.h
    object.h
    paraboloid.h
    )

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(opengl_morphing ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(opengl_morphing application)
TARGET_LINK_LIBRARIES(opengl_morphing application)
