#include "../avx2_vec.h"

int main()
{
    avx2_int32vec_t input1;

    std::cout << "Unsafe multiply 2 vector(int32_t)[" << avx2_int32vec_t::size() << "]\n";

    read("input1", input1);
    std::int32_t scale;
    std::cout << "scale: ";
    std::cin >> scale;

    auto avx1 = _mm256_load_si256(&input1);
    auto avx2 = _mm256_set1_epi32(scale);
    auto avxr = _mm256_mullo_epi32(avx1, avx2);

    avx2_int32vec_t result;
    _mm256_store_si256(&result, avxr);
    trace("result", result);

    return 0;
}
