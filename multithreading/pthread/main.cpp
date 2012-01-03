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

#include <pthread.h>
#include <iostream>

void* task(void* x)
{
    std::cout << "Create thread" << std::endl;
    int *temp;
    temp = static_cast<int*>(x);
    std::cout << "Create task with num:  " << *temp << std::endl;
    std::cout << "Thread id:  " << pthread_self() << std::endl;
    std::cout << "Thread is completed" << std::endl;
}

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;
    
    pthread_t thread_a, thread_b, thread_c;
    
    int n1 = 6;
    int n2 = 7;
    int n3 = 8;
    
    pthread_create(&thread_a, NULL, task, &n1);
    pthread_create(&thread_b, NULL, task, &n2);
    pthread_create(&thread_c, NULL, task, &n3);
    pthread_detach(thread_c);
    
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    
    return 0;
}
