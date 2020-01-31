/*
 *  Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <cmath>
#include <array>
#include <sstream>
#include <cstdint>
#include <iostream>

using type = std::uint64_t;

void output_number(type num, type base);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "usage: app <num> <base>" << std::endl;
        return 1;
    }

    std::stringstream sstream;
    sstream << argv[1] << ' ' << argv[2];

    type num, base;
    sstream >> num >> base;

    if (base >= 2 and base <=16)
    {
        output_number(num, base);
        return 0;
    }
    else
    {
        std::cout << "only 2 <= base <= 16 supported" << std::endl;
        return 2;
    }
}

constexpr char alphabet[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void output_number(type num, type base)
{
    std::array<std::uint8_t, sizeof(type) * 8> array{};

    int i = 0;
    for (; num != 0; i++)
    {
        array[i] = num % base;
        num /= base;
    }

    for (int j = std::ceil(sizeof(type) * 8 / std::log2(base)) - 1; j >= 0; j--)
    {
        std::cout << alphabet[array[j]];
    }
    std::cout << std::endl;
}
