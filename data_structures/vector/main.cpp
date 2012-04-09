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

#include <iostream>

#include <vector.h>

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    vector<int> v2(10, 50);
    
    for(int i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << std::endl;
    }
    
    v2.resize(20, 30);
    
    const int tc = v2.at(3);
    int tn = v2.at(3);
    
    std::cout << "!!!!" << std::endl;
    
    long long size1 = v2.size();
    long long cap1 = v2.capacity();
    long long max1 = v2.max_size();
    
    for(int i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << std::endl;
    }
    
    int limit = v2.size() * 10;
    for(int i = 0; i < limit; ++i) {
        v2.push_back(i);
    }
    
    std::cout << "!!!!" << std::endl;
    
    int size = v2.size();
    for(int i = 0; i < size; ++i) {
        std::cout << v2[i] << std::endl;
    }
    
    long long size2 = v2.size();
    long long cap2 = v2.capacity();
    long long max2 = v2.max_size();
    
    return 0;
}
