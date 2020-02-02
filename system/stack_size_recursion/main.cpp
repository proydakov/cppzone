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

template<class T>
struct node
{
    node() : data(), next(nullptr) {}

    T     data;
    node* next;
};

template<class T>
void clear(node<T>* parent)
{
    if(parent != nullptr) {
        clear(parent->next);
    }
    delete parent;
}

typedef node<size_t> inode;

int main()
{
    constexpr size_t size = 1024 * 1024 * 8;

    inode* parent = new inode;
    inode* previos = parent;
    for(size_t i = 1; i < size; i++) {
        inode* current = new inode;
        current->data = i;
        previos->next = current;
        previos = current;
    }

    std::cout << "init" << std::endl;

    clear(parent);

    std::cout << "destroy" << std::endl;

    return 0;
}
