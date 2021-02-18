#==============================================================================
# Copyright (c) 2011-2020 Evgeny Proydakov <e.proydakov@gmail.com>
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)

PROJECT(I++)
MESSAGE(STATUS "CREATE PROJECT I++")

INCLUDE(cmake/icmake.cmake)

# SETUP CODE FOLDERS

ADD_SUBDIRECTORY(asm)
ADD_SUBDIRECTORY(algorithms)

ADD_SUBDIRECTORY(stl)
ADD_SUBDIRECTORY(syntax)
ADD_SUBDIRECTORY(system)
ADD_SUBDIRECTORY(task)
ADD_SUBDIRECTORY(template)
ADD_SUBDIRECTORY(intrinsic)
ADD_SUBDIRECTORY(multithreading)

IF(MSVC)
    SET(BOOST_ROOT ${PROJECT_SOURCE_DIR}/thirdparty/windows/boost_1_73_0_msvc_2019/)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF()
IF(STATIC_LINK)
    SET(Boost_USE_STATIC_LIBS    ON)
ENDIF()
FIND_PACKAGE(Boost COMPONENTS thread system date_time regex chrono coroutine context graph)
IF(Boost_FOUND)
    ADD_SUBDIRECTORY(asio)
    ADD_SUBDIRECTORY(boost)
ENDIF()

IF(PROJECT_OS_LINUX)
    ADD_SUBDIRECTORY(linux)
ENDIF()

SET(OpenGL_GL_PREFERENCE "GLVND")
FIND_PACKAGE(SDL2)
FIND_PACKAGE(OpenGL)
IF(NOT STATIC_LINK AND SDL2_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY(opengl)
ENDIF()

IF(UNIX AND NOT STATIC_LINK AND SDL2_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY(opengles2)
ENDIF()

IF(PROJECT_OS_WIN)
    ADD_SUBDIRECTORY(nt)
ENDIF()

SET(FILES
    ${PROJECT_SOURCE_DIR}/README.md
    ${PROJECT_SOURCE_DIR}/COPYING
)

#==============================================================================
#---------------------------------- info --------------------------------------
#==============================================================================

I_BUILD_REPORT()

#==============================================================================

IF(PROJECT_OS_WIN)
    SET(SDL2_BINARIES ${PROJECT_SOURCE_DIR}/thirdparty/windows/SDL2/lib/${GLOBAL_SEARCH_SUFFIX}/SDL2.dll)

    SET(DLL_FILES
        ${SDL2_BINARIES}
    )

    FILE(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    FILE(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
ENDIF()
