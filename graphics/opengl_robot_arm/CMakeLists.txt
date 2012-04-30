#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "create  opengl  robot  arm  graphics")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

FIND_PACKAGE(GLUT)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})

FIND_PACKAGE(OpenGL)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(SOURCE_FILES
    main.cpp)

I_ADD_EXECUTABLE(graphics_opengl_robot_arm ${HEADER_FILES} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(graphics_opengl_robot_arm ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(graphics_opengl_robot_arm ${GLUT_LIBRARIES})
TARGET_LINK_LIBRARIES(graphics_opengl_robot_arm ${Boost_THREAD_LIBRARY})
