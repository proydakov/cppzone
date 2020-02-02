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

IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF()
FIND_PACKAGE(Boost COMPONENTS thread system date_time regex chrono coroutine context graph)
IF(Boost_FOUND)
    ADD_SUBDIRECTORY(asio)
    ADD_SUBDIRECTORY(boost)
ENDIF()

ADD_SUBDIRECTORY(multithreading)
ADD_SUBDIRECTORY(stl)
ADD_SUBDIRECTORY(syntax)
ADD_SUBDIRECTORY(system)
ADD_SUBDIRECTORY(task)
ADD_SUBDIRECTORY(template)

IF(PROJECT_OS_WIN)
    ADD_SUBDIRECTORY(nt)

#    FIND_PACKAGE(DIRECTX9)
#    IF(DIRECTX9_FOUND AND MSVC)
#        ADD_SUBDIRECTORY(D3DX9)
#    ENDIF()
ENDIF()

IF(PROJECT_OS_LINUX)
    ADD_SUBDIRECTORY(linux)
ENDIF()

FIND_PACKAGE(GLUT)
FIND_PACKAGE(OpenGL)
IF(GLUT_FOUND AND OPENGL_FOUND)
    ADD_SUBDIRECTORY(opengl)
ENDIF()

FIND_PACKAGE(Lua51)
IF(LUA51_FOUND)
    ADD_SUBDIRECTORY(lua)
ENDIF()

SET(FILES
    ${PROJECT_SOURCE_DIR}/README.md
    ${PROJECT_SOURCE_DIR}/COPYING
    )

#==============================================================================
#---------------------------------- info --------------------------------------
#==============================================================================

# build info
SET(BUILD_INFO_BAR "====================================================================================")
SET(NOOP_STRING "")

MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "Summary of the build:")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS "GENERATOR:    ${CMAKE_GENERATOR}")
MESSAGE(STATUS "C_COMPILER:   ${CMAKE_C_COMPILER} [${CMAKE_C_COMPILER_ID}-${CMAKE_C_COMPILER_VERSION}]")
MESSAGE(STATUS "CXX_COMPILER: ${CMAKE_CXX_COMPILER} [${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}]")
MESSAGE(STATUS "LINKER:       ${CMAKE_LINKER}")
MESSAGE(STATUS "")
MESSAGE(STATUS "Build type : ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS ${BUILD_INFO_BAR})
MESSAGE(STATUS ${NOOP_STRING})

#==============================================================================

IF(PROJECT_OS_WIN)

    SET(SDL2_BINARIES ${PROJECT_SOURCE_DIR}/thirdparty/windows/SDL2/lib/${GLOBAL_SEARCH_SUFFIX}/SDL2.dll)
    SET(GLUT_BINARIES ${PROJECT_SOURCE_DIR}/thirdparty/windows/freeglut/bin/${GLOBAL_SEARCH_SUFFIX}/freeglut.dll)
    SET(LUA_BINARIES  ${PROJECT_SOURCE_DIR}/thirdparty/windows/Lua5.1/lib/${GLOBAL_SEARCH_SUFFIX}/lua5.1.dll)

    SET(DLL_FILES
        ${SDL2_BINARIES}
        ${GLUT_BINARIES}
        ${LUA_BINARIES}
    )

    FILE(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    FILE(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

ENDIF()
