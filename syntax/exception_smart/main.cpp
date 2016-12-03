/*
 *  Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <iostream>

class smart_error
{
public:
    smart_error(int val) : val(val)
    {
        std::cout << "smart_error(create): " << val << std::endl;
    }

    smart_error(const smart_error& error)
    {
        val = error.val;
        std::cout << "smart_error(copy): " << val << std::endl;
    }

    ~smart_error()
    {
        std::cout << "smart_error(destroy): " << val << std::endl;
    }

    void set_val(int val)
    {
        val = val;
    }

private:
    int val;
};

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    try {
        throw smart_error(1);
    }
    catch(smart_error error) {
        error.set_val(3);
        throw error;
    }
    catch(smart_error error) {
    }

    return 0;
}
