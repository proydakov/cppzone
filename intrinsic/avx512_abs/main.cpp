#include "../avx512_vec.h"

int main()
{
    avx512_int32vec_t input1;

    std::cout << "Abs vector(int32_t)[" << avx512_int32vec_t::size() << "]\n";

    read("input1", input1);

    auto avx1 = _mm512_load_si512(&input1);
    auto avxr = _mm512_abs_epi32(avx1);

    avx512_int32vec_t result;
    _mm512_store_si512(&result, avxr);
    trace("result", result);

    return 0;
}
