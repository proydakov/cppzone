#include <iostream>
#include <immintrin.h>

template<typename Type>
struct alignas(64) avx512_t
{
    static constexpr size_t size() noexcept
    {
        return 64 / sizeof(Type);
    }

    Type val[size()];

    static_assert(sizeof(val) == 64, "AVX512 require 64 byte alignment for data types.");
};

template<typename AVX512VEC>
void read(char const * const name, AVX512VEC& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

template<typename AVX512VEC>
void trace(char const * const name, AVX512VEC const& vec)
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
    using avx512_int32vec_t = avx512_t<int>;

    using raw_ptr = __m512i*;
    using craw_ptr = __m512i const*;

    avx512_int32vec_t input1;
    avx512_int32vec_t input2;

    std::cout << "And 2 vector(int32_t)[" << avx512_int32vec_t::size() << "]\n";

    read("input1", input1);
    read("input2", input2);

    auto avx1 = _mm512_load_si512(reinterpret_cast<craw_ptr>(&input1));
    auto avx2 = _mm512_load_si512(reinterpret_cast<craw_ptr>(&input2));
    auto avxr = _mm512_and_si512(avx1, avx2);

    avx512_int32vec_t result;
    _mm512_store_si512(reinterpret_cast<raw_ptr>(&result), avxr);
    trace("result", result);

    return 0;
}
