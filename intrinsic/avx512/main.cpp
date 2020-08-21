#include <iterator>
#include <iostream>
#include <immintrin.h>

template<typename Type, size_t Size>
struct alignas(64) avx512_t
{
    constexpr size_t size() const noexcept
    {
        return Size;
    }

    Type val[Size];

    static_assert(sizeof(val) == 64, "AVX512 require 64 byte alignment for data types.");
};

using avx512_int32vec_t = avx512_t<int, 16>;

void read(const char* name, avx512_int32vec_t& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

void trace(const char* name, avx512_int32vec_t const& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec.val[i] << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    using raw_ptr = __m512i*;
    using craw_ptr = __m512i const*;

    avx512_int32vec_t input1;
    avx512_int32vec_t input2;

    read("input1", input1);
    read("input2", input2);

    auto avx1 = _mm512_load_si512(reinterpret_cast<craw_ptr>(&input1));
    auto avx2 = _mm512_load_si512(reinterpret_cast<craw_ptr>(&input2));
    auto avx  = _mm512_add_epi32(avx1, avx2); 

    avx512_int32vec_t result;
    _mm512_store_si512(reinterpret_cast<raw_ptr>(&result), avx);
    trace("result", result);

    return 0;
}
