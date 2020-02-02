/*
 *  Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
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
#include <unistd.h>
#include <sys/mman.h>

#include <string>
#include <iostream>
#include <sstream>

//
// example: ./unix_hugepages 1024 -1
//
int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cout << "usage: app <size mb> <hugepages -1/0/1>" << std::endl;
        return 1;
    }

    const size_t size = std::stoul(argv[1]) * 1024 * 1024;
    const int hugepages = std::stoi(argv[2]);

    void* p = ::mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(hugepages < 0) {
        std::cout << "<< default behavior" << std::endl;
        // default behavior
    }
    else if(hugepages == 0) {
        std::cout << "<< madvise MADV_NOHUGEPAGE" << std::endl;
        madvise(p, size, MADV_NOHUGEPAGE);
    }
    else {
        std::cout << "<< madvise MADV_HUGEPAGE" << std::endl;
        madvise(p, size, MADV_HUGEPAGE);
    }

    for (size_t* i = ((size_t*)p) + 1; i < (size_t*)((char*)p + size); i++) {
        size_t z = *(i - 1);
        *i ^= z;
    }
    std::cout << "<< mmap ptr: " << std::hex << p << std::endl;

    std::ostringstream ss;
    ss << "cat /proc/" << ::getpid() << "/smaps | grep -A 18 " << std::hex << (size_t)p;

    std::cout << ss.str() << std::endl;
    int call = ::system(ss.str().c_str());

    ::sleep(1'000'000);

    return call;
}
