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

#include <string.h>
#include <iostream>

#include "lua.hpp"

#define LIB_EXPORT extern "C" __attribute__((visibility("default")))

static int fun(lua_State* L)
{
    (void)L;
    std::cout << "* fun *" << std::endl;
    return 0;
}

static int fun_fix(lua_State* L)
{
    const char* text1 = lua_tostring(L, 1);
    const int num     = lua_tonumber(L, 2);
    const char* text2 = lua_tostring(L, 3);
    
    std::cout << "* fun_fix *" << std::endl;
    std::cout << "text1  -  " << text1 << std::endl;
    std::cout << "num    -  " << num << std::endl;
    std::cout << "text2  -  " << text2 << std::endl;
    
    return 0;
}

static int fun_var(lua_State* L)
{
    const int count = lua_gettop(L);
    std::cout << "* fun_var *" <<  std::endl << "arguments count: " << count << std::endl;
    std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
    for(int i = 1; count >= i; ++i) {
        std::cout << "fun_var argument is " << i << " | ";
        int type = lua_type(L, i);
        std::cout << "type : ";
        std::cout.width(10);
        std::cout << lua_typename(L, type) << "  | value:  ";
        std::cout.width(10);
        switch(type) {
        case 1:
            std::cout << lua_toboolean(L, i);
            break;
        case 3:
            std::cout << lua_tonumber(L, i);
            break;
        case 4:
            std::cout << lua_tostring(L, i);
            break;
        default:
            std::cout << "invalid argument type";
            break;
        }
        std::cout << std::endl;
    }
    return 0;
}

static int fun_return_num(lua_State* L)
{
    lua_pushnumber(L, 777);
    return 1;
}

static int fun_return_string(lua_State* L)
{
    lua_pushstring(L, "hello, my name C");
    return 1;
}

static int fun_return_set(lua_State* L)
{
    lua_pushstring(L, "start");
    for(int i = 100; i < 103; ++i) {
        lua_pushnumber(L, i);
    }
    lua_pushstring(L, "end");
    return 5;
}

static const luaL_Reg lua_possible[] = 
{
    {"fun", fun},
    {"fun_fix", fun_fix},
    {"fun_var", fun_var},
    {"fun_return_num", fun_return_num},
    {"fun_return_string", fun_return_string},
    {"fun_return_set", fun_return_set},
    {0, 0}
};

LIB_EXPORT int luaopen_libcapabilities(lua_State* L)
{
    luaL_register(L, "libcapabilities", lua_possible);
    return 0;
}
