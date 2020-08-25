#include <immintrin.h>
#include "../avx512_vec.h"

int main()
{
    using avx512_int32vec_t = avx512_t<int>;

    using raw_ptr = __m512i*;
    using craw_ptr = __m512i const*;

    avx512_int32vec_t input1;

    std::cout << "Negative vector(int32_t)[" << avx512_int32vec_t::size() << "]\n";

    read("input1", input1);

    auto avx1 = _mm512_load_si512(reinterpret_cast<craw_ptr>(&input1));
    auto avx_zero = _mm512_setzero_si512();
    auto avx_mask = _mm512_int2mask(0b1111111111111111);
    auto avxr = _mm512_mask_sub_epi32(avx1, avx_mask, avx_zero, avx1);

    avx512_int32vec_t result;
    _mm512_store_si512(reinterpret_cast<raw_ptr>(&result), avxr);
    trace("result", result);

    return 0;
}
