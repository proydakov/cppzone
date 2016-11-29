#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Lua51)
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

#==============================================================================

SET(LUA_SCRIPT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

SET(LUA_FILES
    ${LUA_SCRIPT_DIRECTORY}/burn.lua
    ${LUA_SCRIPT_DIRECTORY}/mem1.lua
    ${LUA_SCRIPT_DIRECTORY}/mem2.lua
    )

SET(MAIN_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(lua_alloc ${MAIN_FILES} ${LUA_FILES})
TARGET_LINK_LIBRARIES(lua_alloc ${LUA_LIBRARIES})
