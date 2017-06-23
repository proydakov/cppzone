#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <iostream>

// Выделяет RW память заданного размера и возвращает указатель на нее. В случае ошибки
// печатает ошибку и возвращает NULL. В отличие от malloc, память выделяется
// на границе страниц памяти, так что ее можно использовать при вызове mprotect.
void* alloc_writable_memory(size_t size) {
    void* ptr = mmap(0, size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0
    );

    if (ptr == (void*)-1) {
        perror("mmap");
        return NULL;
    }
    return ptr;
}

// Ставит RX права на этот кусок выровненной памяти. Возвращает
// 0 при успехе. При ошибке печатает ошибку и возвращает -1.
int make_memory_executable(void* m, size_t size) {
    if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect");
        return -1;
    }
    return 0;
}

void emit_code_into_memory(void* m) {
    unsigned char code[] = {
        0x48, 0x8d, 0x47, 0x04,             // lea    rax,[rdi+0x4]
        0xc3                                // ret
    };
    memcpy(m, code, sizeof(code));
}

// Выделяет RW память, сохраняет код в нее и меняет права на RX перед
// исполнением.
void emit_to_rw_run_from_rx(long val) {
    const size_t SIZE = 1024;
    typedef long (*JittedFunc)(long);

    void* m = alloc_writable_memory(SIZE);
    emit_code_into_memory(m);
    make_memory_executable(m, SIZE);

    JittedFunc func = (JittedFunc)( m );
    int result = func(val);
    printf("result = %d\n", result);

    const int unm = munmap(m, SIZE);
    if(unm == -1) {
        perror("munmap");
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cout << "usage: " << argv[0] << " <val>" << std::endl;
        return 1;
    }

    const long val = std::stol(argv[1]);
    emit_to_rw_run_from_rx(val);

    return 0;
}
