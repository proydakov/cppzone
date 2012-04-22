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

#include <time.h>
#include <cstdlib>
#include <iostream>

#include <binary_tree.h>

typedef unsigned int index;
typedef binary_tree<index> ctree;

void print_ascending(const ctree& tree);
void print_descending(const ctree& tree);

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    ctree tree;
    
    std::cout << "SIZE : " << sizeof(ctree) << std::endl;
    
    index size = 15;
    srand(time(NULL));
    for(index i = 0; i < size; ++i) {
        if(!tree.insert(rand() % size + 1))
            return 1;
    }
    
    tree.print();
    print_ascending(tree);
    print_descending(tree);
    
    //tree.print();
    for(index i = 0; i < size / 2; ++i)
        tree.erase(rand() % size);
    //tree.print();
    
    return 0;
}

void print_ascending(const ctree& tree)
{
    const ctree::node* current = tree.min();
    
    std::cout << "!!!!!!successor!!!!!!" << std::endl;
    while(current != NULL_PTR) {
        ctree::print_node(current);
        std::cout << std::endl;
        current = tree.successor(current);
    }
}

void print_descending(const ctree& tree)
{
    const ctree::node* current = tree.max();
    
    std::cout << "!!!!!!predecessor!!!!!!" << std::endl;
    while(current != NULL_PTR) {
        ctree::print_node(current);
        std::cout << std::endl;
        current = tree.predecessor(current);
    }
}
