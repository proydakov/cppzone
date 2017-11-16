#include <memory>
#include <cstdint>
#include <iostream>

struct alignas(32) s32
{
    s32()
    {
        std::cout << "align s32 ok: " << ((intptr_t)this % 32 == 0) << std::endl;
    }

    int val;
};

struct alignas(64) s64
{
    s64()
    {
        std::cout << "align s64 ok: " << ((intptr_t)this % 64 == 0) << std::endl;
    }

    int val;
};

struct alignas(8) S
{
    S()
    {
        std::cout << "t*: " << (void*)(this) << std::endl;
    }

    char  c1;
    short s1;
    int   i1;
    char  c2;
};

#pragma pack(push, 1)
struct spack
{
    char c1;
    int  i1;
    char c2;
    int  i2;
};
#pragma pack(pop)

int main()
{
    std::ios::sync_with_stdio(false);

    std::cout << "sizeof:      " << std::endl;
    std::cout << "sizeof(S):   " << sizeof(S) << std::endl;
    std::cout << "sizeof(s32): " << sizeof(s32) << std::endl;
    std::cout << "sizeof(s64): " << sizeof(s64) << std::endl;

    std::cout << std::endl;

    std::cout << "std::alignment_of:" << std::endl;
    std::cout << "char:    " << std::alignment_of<char>::value << std::endl;
    std::cout << "int32_t: " << std::alignment_of<int32_t>::value << std::endl;
    std::cout << "int64_t: " << std::alignment_of<int64_t>::value << std::endl;
    std::cout << "S:       " << std::alignment_of<S>::value << std::endl;
    std::cout << "spack:   " << std::alignment_of<spack>::value << std::endl;
    std::cout << "s32:     " << std::alignment_of<s32>::value << std::endl;
    std::cout << "s64:     " << std::alignment_of<s64>::value << std::endl;

    std::cout << std::endl;

    std::cout << "alignof:" << std::endl;
    std::cout << "char:    " << alignof(char) << std::endl;
    std::cout << "int32_t: " << alignof(int32_t) << std::endl;
    std::cout << "int64_t: " << alignof(int64_t) << std::endl;
    std::cout << "S:       " << alignof(S) << std::endl;
    std::cout << "spack:   " << alignof(spack) << std::endl;
    std::cout << "s32:     " << alignof(s32) << std::endl;
    std::cout << "s64:     " << alignof(s64) << std::endl;

    std::cout << std::endl;

    std::cout << "detail(S): " << std::endl;
    S data;
    std::cout << "d*: " << (void*)&data << std::endl;
    std::cout << "c1: " << (void*)&data.c1 << std::endl;
    std::cout << "s1: " << (void*)&data.s1 << std::endl;
    std::cout << "i1: " << (void*)&data.i1 << std::endl;
    std::cout << "c2: " << (void*)&data.c2 << std::endl;

    std::cout << std::endl;

    std::cout << "instance test" << std::endl;
    s32 data32;
    s64 data64;

    auto ptr32 = std::unique_ptr<s32>(new s32());
    auto ptr64 = std::unique_ptr<s64>(new s64());

    return 0;
}
