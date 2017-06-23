//
// g++ -std=c++14 main.cpp -o app
//

#include <array>
#include <cstdint>
#include <iostream>

volatile uint64_t g_stack_trap = 0;

#define PUT_TRAP()                                              \
    volatile uint64_t head = 777444111;                         \
    g_stack_trap = (uint64_t)&head;                             \

#define TRACE_STACK_REGS()                                      \
    register uint64_t rbp asm ("rbp");                          \
    register uint64_t rsp asm ("rsp");                          \
                                                                \
    std::cout << "rbp:  " << std::hex << rbp << std::endl;      \
    std::cout << "rsp:  " << std::hex << rsp << std::endl;      \

#define TRACE_STACK_VALS()                                      \
    register uint64_t rbp asm ("rbp");                          \
    register uint64_t rsp asm ("rsp");                          \
                                                                \
    std::cout << "rbp:  " << std::hex << rbp << std::endl;      \
    std::cout << "rsp:  " << std::hex << rsp << std::endl;      \
                                                                \
    std::cout << "STACK CHAR" << std::endl;                     \
    for(char* i = (char*)g_stack_trap; i >= (char*)rsp; i--) {  \
        std::cout << std::hex << "ptr: " << (uint64_t*)i        \
                  << std::dec << " val: " << (char)*i           \
                  << std::endl;                                 \
    }                                                           \
                                                                \
    std::cout << "STACK UINT64_t" << std::endl;                 \
    for(uint64_t* i =(uint64_t*)g_stack_trap; i >= (uint64_t*)rsp; i--) { \
        std::cout << std::hex << "ptr: " << i                   \
                  << std::dec << " val: " << *i                 \
                  << std::endl;                                 \
    }                                                           \
                                                                \

int make_work()
{
    std::array<char, 28> array;
    array.fill('0');
    for(int i = 1; i < array.size() - 1; i++) {
        array[i] = char('a' + i - 1);
    }
    array.front() = '*';
    array.back () = '*';

    TRACE_STACK_VALS();

    return array.size();
}

int main()
{
    std::ios::sync_with_stdio(false);

    PUT_TRAP();

    std::cout << "TRAP: " << std::hex << g_stack_trap << ' '
              << "VAL: " << std::dec << (*(uint64_t*)g_stack_trap)
              << std::endl;

    int code = make_work();

    TRACE_STACK_REGS();

    return code + (int)(head);
}
