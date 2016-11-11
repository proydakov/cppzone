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
#include <sstream>
#include <iostream>

#include <lua.hpp>

void load_lua_file(lua_State *L, const char* name);
void check_execution(lua_State *L);

void test_memory(const std::string& script);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " path/to/file" << std::endl;
        return 1;
    }

    std::string ifile(argv[1]);

    test_memory(ifile);

    return 0;
}

void load_lua_file(lua_State* L, const std::string& name)
{
    std::cout << "[C++] " << "Loading Lua script: " << name << std::endl;
    int result = luaL_loadfile(L, name.c_str());
    if (result) {
        std::cout << "[C++] Returned result: " << result << std::endl;
        std::cout << "[C++] Loading error: " << lua_tostring(L, -1) << std::endl;
        exit(1);
    }
}

void check_execution(lua_State* L)
{
    std::cout << "[C++] " << "Starting script" << "\n" << std::endl;
    int result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        std::cout << "[C++] Returned result: " << result << std::endl;
        std::cout << "[C++] Runtime error: " << lua_tostring(L, -1) << std::endl;
        exit(1);
    }
}

void test_memory(const std::string& script)
{
    lua_State* L;
    L = luaL_newstate();
    lua_gc(L, LUA_GCSTOP, 0);

    luaopen_base(L);
    luaopen_table(L);

    load_lua_file(L, script);
    check_execution(L);

    std::cout << "MEMORY USAGE LUA_GCCOUNT: " << lua_gc(L, LUA_GCCOUNT, 0) << std::endl;
    std::cout << "MEMORY USAGE LUA_GCCOUNTB: " << lua_gc(L, LUA_GCCOUNTB, 0) << std::endl;

    int i = 0;
    std::cin >> i;

    lua_close(L);

    std::cout << std::endl;
}
