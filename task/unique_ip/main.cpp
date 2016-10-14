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

#include <set>
#include <cstdint>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <initializer_list>

namespace ip {

template<class T, int Size>
struct impl
{
    impl(std::initializer_list<T> list) {
        if(list.size() != Size) {
            throw std::runtime_error("Invalid init ip");
        }
        size_t i = 0;
        for(auto it = list.begin(); it != list.end(); ++it) {
            data[i] = *it;
            i++;
        }
    }

    bool operator<(const impl& e) const {
        size_t size = sizeof(data) / sizeof(data[0]);
        size_t pos = 0;
        for(size_t i = 0; i < size; i++) {
            pos = size - i - 1;
            if(data[pos] < e.data[pos]) {
                return true;
            }
            else if(data[pos] > e.data[pos]) {
                return false;
            }
        }
        return false;
    }

    T data[Size];
};

typedef impl<uint8_t, 4> v4;
typedef impl<uint16_t, 8> v6;

}

std::ostream& operator<<(std::ostream& os, const ip::v4& ip) {
    for(size_t i = 0; i < sizeof(ip.data) / sizeof(ip.data[0]); i++) {
        if(i > 0) {
            os << ".";
        }
        os << static_cast<int>(ip.data[i]);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const ip::v6& ip) {
    os << std::hex;
    for(size_t i = 0; i < sizeof(ip.data) / sizeof(ip.data[0]); i++) {
        if(i > 0) {
            os << ":";
        }
        os << static_cast<int>(ip.data[i]);
    }
    os << std::dec;
    return os;
}

int main( )
{
    std::set<ip::v4> set_v4;

    set_v4.insert(ip::v4({127,   0, 0, 1}));
    set_v4.insert(ip::v4({192, 168, 0, 1}));
    set_v4.insert(ip::v4({192, 168, 0, 2}));
    set_v4.insert(ip::v4({127,   0, 0, 1}));
    set_v4.insert(ip::v4({192, 168, 0, 1}));

    std::cout << "set ip::v4: \n";
    for(auto it = set_v4.cbegin(); it != set_v4.cend(); ++it) {
        std::cout << *it << "\n";
    }

    std::cout << "\n";

    std::set<ip::v6> set_v6;

    set_v6.insert(ip::v6({0x2001, 0x0db8, 0x11a3, 0x09d7, 0x1f34, 0x8a2e, 0x07a0, 0x765d}));
    set_v6.insert(ip::v6({0x2001, 0x0db8, 0x11a3, 0x09d7, 0x1f34, 0x8a2e, 0x07a0, 0x766d}));
    set_v6.insert(ip::v6({0x2001, 0x0db8, 0x11a3, 0x09d7, 0x1f34, 0x8a2e, 0x07a0, 0x767d}));
    set_v6.insert(ip::v6({0x2001, 0x0db8, 0x11a3, 0x09d7, 0x1f34, 0x8a2e, 0x07a0, 0x766d}));

    std::cout << "set ip::v6: \n";
    for(auto it = set_v6.cbegin(); it != set_v6.cend(); ++it) {
        std::cout << *it << "\n";
    }

    return 0;
}
