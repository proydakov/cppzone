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

#include <string>
#include <iostream>

typedef std::string symbol_container;

void generate_permutation(const symbol_container& data);
void generate_permutation_recursive_step(symbol_container& data, size_t start, size_t end);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;    
    
    symbol_container ex1 = "012345";
    generate_permutation(ex1);
    
    return 0;
}

void generate_permutation(const symbol_container& data)
{
    symbol_container temp = data;
    generate_permutation_recursive_step(temp, 0, temp.size() - 1);
}

void generate_permutation_recursive_step(symbol_container& data, size_t start, size_t end)
{
    if(start == end) {
        std::cout << data << std::endl;
        return;
    }
	for (size_t i = start; i <= end; ++i) {
        std::swap(data[i], data[start]);
        symbol_container temp = data;
        generate_permutation_recursive_step(temp, start + 1, end);
    }
}
