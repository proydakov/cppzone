--[[
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
]]

cap = require("capabilities")
cap = capabilities

cap.fun()                                       -- example of a function call with no parameters
cap.fun_fix('I love Lua!', 100, "la-la-la")     -- example of a function call with no parameters
cap.fun_var(true, 300, "web", 'bob', "robot")   -- example of a function call with a variable number of parameters

print("[Lua] call cap.fun_return_num():    ", cap.fun_return_num()   )   -- example of a function call returns the number
print("[Lua] call cap.fun_return_string(): ", cap.fun_return_string())   -- example of a function call returns the string
print("[Lua] call cap.fun_return_set():    ", cap.fun_return_set()   )   -- example of a function call returns a set of values
