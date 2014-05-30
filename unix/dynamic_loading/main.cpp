/*
 *  Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <dlfcn.h>
#include <sstream>
#include <iostream>

#include "object.h"

void load_library()
{
#ifdef __APPLE__
    char* name = (char*)("libpthread.dylib");
#else
    char* name = (char*)("libpthread.so.0");
#endif
    void* library = dlopen(name, RTLD_LAZY);
    if (library == nullptr) {
        std::cerr << "load library: " << name << " error" << std::endl;
        exit(1);
    }
    std::cout << "load library: " << name << " - ok. adress: " << library << std::endl;
    dlclose(library);
}

void load_object()
{
#ifdef __APPLE__
    char* name = (char*)("libunix_object.dylib");
#else
    char* name = (char*)("libunix_object.so");
#endif
    void* library = dlopen(name, RTLD_LAZY);
    if (library == NULL) {
        std::cerr << "load library: " << name << " error" << std::endl;
        exit(1);
    }
    std::cout << "load library: " << name << " - ok. adress: " << library << std::endl;

    create_object_ptr create;
    destroy_object_ptr destroy;

    create  = (create_object_ptr) dlsym(library, "create_object");
    destroy = (destroy_object_ptr) dlsym(library, "destroy_object");

    if(create == nullptr) {
        std::cerr << "load: " << "create_object" << " error" << std::endl;
        exit(1);
    }
    if(destroy == nullptr) {
        std::cerr << "load: " << "destroy_object" << " error" << std::endl;
        exit(1);
    }

    object* obj = (object*) create();
    obj->call();
    destroy( obj );

    dlclose(library);
}

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    load_library();
    load_object();

    return 0;
}
