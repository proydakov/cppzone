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

#include "lua.hpp"

#define TERMINATION_NORMAL  EXIT_SUCCESS
#define TERMINATION_ERROR   1

typedef std::string string;

const string PARENT_DIRECTORY_STRING = PARENT_DIRECTORY;

void load_lua_file(lua_State *L, const char* name);
void check_execution(lua_State *L);

void get_config();
void call_function_from_c();
void reload();

int main(void)
{
    std::cout << "--config--" << std::endl;
    get_config();
    std::cout << std::endl << "--call C--" << std::endl;
    call_function_from_c();
    std::cout << std::endl << "--reload--" << std::endl;
    reload();
    return TERMINATION_NORMAL;
}

void load_lua_file(lua_State *L, const char* name)
{
    int result = luaL_loadfile(L, name);
    if (result) {
        std::cout << "Returned result: " << result << std::endl;
        std::cout << "Loading error: " << lua_tostring(L, -1) << std::endl;
        exit(TERMINATION_ERROR); 
    } 
}

void check_execution(lua_State *L)
{
    int result = lua_pcall(L, 0, LUA_MULTRET, 0); 
    if (result) {
        std::cout << "Returned result: " << result << std::endl;
        std::cout << "Runtime error: " << lua_tostring(L, -1) << std::endl;
        exit(TERMINATION_ERROR); 
    }
}

void get_config()
{
    lua_State * L;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    string lia_file = PARENT_DIRECTORY_STRING;
    lia_file += "config.lua";
    load_lua_file(L, lia_file.c_str());
    check_execution(L);
    
    lua_getglobal(L, "my_arc");
    lua_getglobal(L, "my_os");
    lua_getglobal(L, "my_name");
    
    std::cout << "my_name: " << lua_tostring(L, -1) << std::endl;
    std::cout << "my_os  : " << lua_tostring(L, -2) << std::endl;
    std::cout << "my_arc : " << lua_tostring(L, -3) << std::endl;
    
    lua_close(L);  
}

void call_function_from_c()
{
    lua_State * L;
    L = luaL_newstate();
    luaL_openlibs(L);
    
    string lia_file = PARENT_DIRECTORY_STRING;
    lia_file += "call_c.lua";
    load_lua_file(L, lia_file.c_str());
    check_execution(L);
    
    lua_close(L);  
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
            lia_file += "cycle.lua";
            load_lua_file(L, lia_file.c_str());
            check_execution(L);
        }
        else {
            break;
        }
        std::cout << "Enter control key 1 (to reload cycle.lua), or 0 (to exit): ";
        std::cin >> command;
    }
    while(command);
    lua_close(L); 
}
