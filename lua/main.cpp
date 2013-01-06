/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <lua.hpp>

#include "config_lua.h"

#define TERMINATION_NORMAL  EXIT_SUCCESS
#define TERMINATION_ERROR   1

#define CONFIG_LUA_SCRIPT "config.lua"
#define CALL_C_LUA_SCRIPT "call_c.lua"
#define RELOAD_LUA_SCRIPT "cycle.lua"

typedef std::string string;

const string PARENT_DIRECTORY_STRING = PARENT_DIRECTORY;

void load_lua_file(lua_State *L, const char* name);
void check_execution(lua_State *L);

void get_config();
void call_function_from_c();
void reload();

int main(void)
{
    get_config();
    call_function_from_c();
    reload();
    return TERMINATION_NORMAL;
}

void load_lua_file(lua_State* L, const char* name)
{
    std::cout << "[C++] " << "Loading Lua script: " << name << std::endl;
    int result = luaL_loadfile(L, name);
    if (result) {
        std::cout << "[C++] Returned result: " << result << std::endl;
        std::cout << "[C++] Loading error: " << lua_tostring(L, -1) << std::endl;
        exit(TERMINATION_ERROR);
    }
}

void check_execution(lua_State* L)
{
    std::cout << "[C++] " << "Starting script" << "\n" << std::endl;
    int result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        std::cout << "[C++] Returned result: " << result << std::endl;
        std::cout << "[C++] Runtime error: " << lua_tostring(L, -1) << std::endl;
        exit(TERMINATION_ERROR);
    }
}

void get_config()
{
    lua_State* L;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    string lia_file = PARENT_DIRECTORY_STRING;
    lia_file += CONFIG_LUA_SCRIPT;
    load_lua_file(L, lia_file.c_str());
    check_execution(L);
    
    lua_getglobal(L, "my_arc");
    lua_getglobal(L, "my_os");
    lua_getglobal(L, "my_name");
    
    std::cout << "[C++] my_name: " << lua_tostring(L, -1) << std::endl;
    std::cout << "[C++] my_os  : " << lua_tostring(L, -2) << std::endl;
    std::cout << "[C++] my_arc : " << lua_tostring(L, -3) << std::endl;
    
    lua_close(L);

    std::cout << std::endl;
}

void call_function_from_c()
{
    lua_State * L;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    string lia_file = PARENT_DIRECTORY_STRING;
    lia_file += CALL_C_LUA_SCRIPT;
    load_lua_file(L, lia_file.c_str());
    check_execution(L);
    
    lua_close(L);

    std::cout << std::endl;
}

void reload()
{
    lua_State * L;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    int command = 1;
    do {
        if(command) {
            string lia_file = PARENT_DIRECTORY_STRING;
            lia_file += RELOAD_LUA_SCRIPT;
            load_lua_file(L, lia_file.c_str());
            check_execution(L);
        }
        else {
            break;
        }
        std::cout << "[C++] " << "Enter control key 1 (to reload cycle.lua), or 0 (to exit): ";
        std::cin >> command;
        if(command) {
            std::cout << std::endl;
        }
    }
    while(command);
    lua_close(L);

    std::cout << std::endl;
}
