#include "../avx512_vec.h"

int main()
{
    avx512_int32vec_t input1;

    std::cout << "Unsafe multiply vector(int32_t)[" << avx512_int32vec_t::size() << "] and scalar\n";

    read("input1", input1);
    std::int32_t scalar;
    std::cout << "scale: ";
    std::cin >> scalar;

    auto avx1 = _mm512_load_si512(&input1);
    auto avx2 = _mm512_set1_epi32(scalar);
    auto avxr = _mm512_mullo_epi32(avx1, avx2);

    avx512_int32vec_t result;
    _mm512_store_si512(&result, avxr);
    trace("result", result);

    return 0;
}
