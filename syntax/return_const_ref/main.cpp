/*
 *  Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <string>
#include <iostream>

class object
{
public:
    object() : m_data("data") { std::cout << "create" << std::endl; }
    object(const object& obj) { std::cout << "copy" << std::endl; }
    ~object() { std::cout << "destroy" << std::endl; }

    const std::string m_data;
};

std::ostream& operator<<(std::ostream& os, const object& obj)
{
    std::cout << "object: " << obj.m_data;
    return os;
}

object make_object_normal()
{
    return object();
}

const object& make_object_error()
{
    const object& ref = make_object_normal();
    std::cout << ref << std::endl;
    return ref;
}

int main()
{
    const object& ref = make_object_error();
    std::cout << ref << std::endl;
    return 0;
}
