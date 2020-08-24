#include <immintrin.h>
#include "../avx2_vec.h"

int main()
{
    using avx2_int32vec_t = avx2_t<int>;

    using raw_ptr = __m256i*;
    using craw_ptr = const __m256i*;

    avx2_int32vec_t input1;
    avx2_int32vec_t input2;

    std::cout << "Max 2 vector(int32_t)[" << avx2_int32vec_t::size() << "]\n";

    read("input1", input1);
    read("input2", input2);

    auto avx1 = _mm256_load_si256(reinterpret_cast<craw_ptr>(&input1));
    auto avx2 = _mm256_load_si256(reinterpret_cast<craw_ptr>(&input2));
    auto avxr = _mm256_max_epi32(avx1, avx2);

    avx2_int32vec_t result;
    _mm256_store_si256(reinterpret_cast<raw_ptr>(&result), avxr);
    trace("result", result);

    return 0;
}
