#include <iostream>
#include <immintrin.h>

template<typename Type>
struct alignas(32) avx2_t
{
    static constexpr size_t size() noexcept
    {
        return 32 / sizeof(Type);
    }

    Type val[size()];

    static_assert(sizeof(val) == 32, "AVX2 require 32 byte alignment for data types.");
};

template<typename AVX2VEC>
void read(const char* name, AVX2VEC& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

template<typename AVX2VEC>
void trace(const char* name, AVX2VEC const& vec)
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
    using avx2_int32vec_t = avx2_t<int>;

    using raw_ptr = __m256i*;
    using craw_ptr = const __m256i*;

    avx2_int32vec_t input1;
    avx2_int32vec_t input2;

    std::cout << "Or 2 vector(int32)[" << avx2_int32vec_t::size() << "]\n";

    read("input1", input1);
    read("input2", input2);

    auto avx1 = _mm256_load_si256(reinterpret_cast<craw_ptr>(&input1));
    auto avx2 = _mm256_load_si256(reinterpret_cast<craw_ptr>(&input2));
    auto avxr = _mm256_or_si256(avx1, avx2);

    avx2_int32vec_t result;
    _mm256_store_si256(reinterpret_cast<raw_ptr>(&result), avxr);
    trace("result", result);

    return 0;
}
