#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  LUA  EXAMPLE
#==============================================================================

MESSAGE(STATUS "BUILD  LUA\n")

SET(PROJECT_FOLDER "lua")

FIND_PACKAGE(Lua51)
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(LUA_POSSIBLE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#==============================================================================

SET(LIB_FILES
    ${LUA_POSSIBLE_DIRECTORY}/lib.cpp
    )

I_ADD_LUA_LIBRARY(capabilities ${LIB_FILES})
TARGET_LINK_LIBRARIES(capabilities ${LUA_LIBRARIES})

#==============================================================================

SET(LUA_SCRIPT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

CONFIGURE_FILE(config.lua.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config_lua.h)

SET(LUA_FILES
    ${LUA_SCRIPT_DIRECTORY}/call_c.lua
    ${LUA_SCRIPT_DIRECTORY}/config.lua
    ${LUA_SCRIPT_DIRECTORY}/cycle.lua
    )

SET(CONFIG_FILES
    config_lua.h
    )

SET(MAIN_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(lua_example ${MAIN_FILES} ${CONFIG_FILES} ${LUA_FILES})
TARGET_LINK_LIBRARIES(lua_example ${LUA_LIBRARIES})

MESSAGE("")
