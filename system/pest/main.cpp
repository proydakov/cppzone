/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <string>
#include <spawn.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    srand(time(NULL));
    pid_t pid;
    posix_spawnattr_t attr;
    posix_spawn_file_actions_t action;
    
    while(true) {
        int *p = new int [36];
        std::string name("pest_");
        name += rand() % 10000;
        char *const param[] = {"param", "hello", "world"};
        if(rand() % 3 == 0) {
            system("./multithreading_load_cpu");
        }
        if(rand() % 3 == 1) {
            posix_spawn(&pid, "./system_pest", &action, &attr, param, NULL);
        }
        if(rand() % 3 == 2) {
            system("./system_pest");   
        }
    }
    return 0;
}
