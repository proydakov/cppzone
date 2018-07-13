#==============================================================================
# Copyright (c) 2011-2018 Evgeny Proydakov <e.proydakov@gmail.com>
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

PROJECT(I++)
MESSAGE(STATUS "CREATE PROJECT I++")

# setup cmake modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/platform/shared)
IF(WIN32)
    SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/platform/windows)
ENDIF()

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
    #SET(CMAKE_BUILD_TYPE Release)
ENDIF()

SET(BUILD_SHARED_LIBS OFF)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET(GLOBAL_PROJECT_FOLDER "null")

INCLUDE(ios)
INCLUDE(imacro)
INCLUDE(icompiler)
INCLUDE(ilinker)

# setup build
SETUP_CXX_FLAGS()

SETUP_LINKER()

# colors =)
set (CMAKE_BUILD_COLOR_MAKEFILE ON)

if (CMAKE_GENERATOR STREQUAL "Ninja")
    # Turn on colored output. https://github.com/ninja-build/ninja/wiki/FAQ
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
endif ()

# end setup build

# @todo. try to found good place for this code
IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( GLOBAL_SEARCH_SUFFIX "x64" )
ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    SET( GLOBAL_SEARCH_SUFFIX "x86" )
ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

IF(MSVC)
    SET(Boost_USE_STATIC_LIBS    ON)
    SET(Boost_USE_MULTITHREADED  ON)
ENDIF()
FIND_PACKAGE(Boost COMPONENTS thread system date_time regex chrono coroutine context graph)

ADD_SUBDIRECTORY(asm)
ADD_SUBDIRECTORY(algorithms)

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

#    FIND_PACKAGE(ALUT)
#    FIND_PACKAGE(OpenAL)
#    IF(ALUT_FOUND AND OPENAL_FOUND)
#        ADD_SUBDIRECTORY(openal)
#    ENDIF()
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
