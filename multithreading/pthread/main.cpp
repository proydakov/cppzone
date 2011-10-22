/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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
