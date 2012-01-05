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
        (void)p;
    }
    return 0;
}
