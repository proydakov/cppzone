#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  hello_opengl  example")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(graphics_hello_opengl ${SOURCE_FILES})

IF(MSVC)
    TARGET_LINK_LIBRARIES(graphics_hello_opengl opengl32.lib)
    TARGET_LINK_LIBRARIES(graphics_hello_opengl glu32.lib)
    TARGET_LINK_LIBRARIES(graphics_hello_opengl glut32.lib)
ELSE()
    TARGET_LINK_LIBRARIES(graphics_hello_opengl ${OPENGL_LIBRARIES})
    TARGET_LINK_LIBRARIES(graphics_hello_opengl ${GLUT_LIBRARIES})
ENDIF(MSVC)
