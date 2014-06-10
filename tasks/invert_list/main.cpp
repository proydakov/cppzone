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

#include <iostream>

template<class T>
struct node
{
    node<T>* next;
    T value;
};

template<class T>
node<T>* invert(node<T>* head)
{
    node<T>* previous = head;
    node<T>* current = nullptr;
    node<T>* next = nullptr;
    if(head) {
        current = head->next;
    }
    if(previous) {
        previous->next = nullptr;
    }
    while(current) {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    return previous;
}

template<class T>
void clear(node<T>* head)
{
    node<T>* remove = head;
    node<T>* next = nullptr;
    while(remove) {
        if(remove) {
            next = remove->next;
        }
        else {
            next = nullptr;
        }
        delete remove;
        remove = next;
    }
}


template<class T>
std::ostream& operator<<(std::ostream& os, const node<T>& head)
{
    node<T>* next = head.next;
    std::cout << "list: [";
    std::cout << head.value;
    while(next) {
        std::cout << " " << next->value;
        next = next->next;
    }
    std::cout << "]";
    return os;
}

int main( )
{
    typedef int element;

    const size_t size = 15;
    auto head = new node<element>();
    head->value = 0;
    auto create_head = head;
    for(size_t i = 1; i <= size; i++) {
        auto next = new node<element>();
        next->next = nullptr;
        next->value = i;
        create_head->next = next;
        create_head = next;
    }
    std::cout << "original:" << std::endl;
    std::cout << *head << std::endl;

    auto inv_head = invert(head);

    std::cout << "inverted:" << std::endl;
    std::cout  << *inv_head << std::endl;

    clear(inv_head);

    return 0;
}
