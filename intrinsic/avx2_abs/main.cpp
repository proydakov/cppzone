#include "../avx2_vec.h"

int main()
{
    avx2_int32vec_t input1;

    std::cout << "Abs vector(int32_t)[" << avx2_int32vec_t::size() << "]\n";

    read("input1", input1);

    auto avx1 = _mm256_load_si256(&input1);
    auto avxr = _mm256_abs_epi32(avx1);

    avx2_int32vec_t result;
    _mm256_store_si256(&result, avxr);
    trace("result", result);

    return 0;
}
