#include <memory>
#include <iostream>

template <std::size_t N>
struct alignas(N) IAllocator
{
    IAllocator() : p(data), sz(N)
    {
        std::cout << "align IAllocator ok: " << ((size_t)(this) % N == 0) << std::endl;
    }

    template <typename T>
    T* aligned_alloc(std::size_t a = alignof(T))
    {
        if (std::align(a, sizeof(T), p, sz))
        {
            T* result = reinterpret_cast<T*>(p);
            p = (char*)p + sizeof(T);
            sz -= sizeof(T);
            return result;
        }
        return nullptr;
    }

    char data[N];
    void* p;
    std::size_t sz;
};

void test_in_stack2()
{
    IAllocator<64> a;
    std::cout << "in stack2: " << a.sz << std::endl;
}

void test_in_stack()
{
    IAllocator<64> a;
    std::cout << "in stack: " << a.sz << std::endl;

    test_in_stack2();
}

int main()
{
    IAllocator<64> a;

    // allocate a char
    char* p1 = a.aligned_alloc<char>();
    if (p1) {
        *p1 = 'a';
    }
    std::cout << "allocated a char at " << (void*)p1 << '\n';

    // allocate an int
    int* p2 = a.aligned_alloc<int>();
    if (p2) {
        *p2 = 1;
    }
    std::cout << "allocated an int at " << (void*)p2 << '\n';

    // allocate an int, aligned at 32-byte boundary
    int* p3 = a.aligned_alloc<int>(32);
    if (p3) {
        *p3 = 2;
    }
    std::cout << "allocated an int at " << (void*)p3 << " (32 byte alignment)\n";

    // create must 28
    std::cout << "sz: " << a.sz << std::endl;

    test_in_stack();

    return 0;
}
