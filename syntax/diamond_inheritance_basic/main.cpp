/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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
#include <diamond_inheritance.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    // create
    std::cout << "create" << std::endl;

    std::cout << std::endl << "CREATE a ";
    A* a = new A;
    std::cout << std::endl  << "CREATE b ";
    B* b = new B;
    std::cout << std::endl  << "CREATE c ";
    C* c = new C;
    std::cout << std::endl  << "CREATE c ";
    D* d = new D;
    
    // call
    std::cout << std::endl << "\ncall\n" << std::endl;
    
    a->fun();
    b->fun();
    c->fun();
    d->fun();
    
    // destroy
    std::cout << std::endl << "destroy" << std::endl;

    std::cout << std::endl  << "DELETE a ";
    delete a;
    std::cout << std::endl  << "DELETE b ";
    delete b;
    std::cout << std::endl  << "DELETE c ";
    delete c;
    std::cout << std::endl  << "DELETE d ";
    delete d;
    
    return 0;
}
