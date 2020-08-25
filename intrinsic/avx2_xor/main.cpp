#include "../avx2_vec.h"

int main()
{
    avx2_int32vec_t input1;
    avx2_int32vec_t input2;

    std::cout << "Or 2 vector(int32)[" << avx2_int32vec_t::size() << "]\n";

    read("input1", input1);
    read("input2", input2);

    auto avx1 = _mm256_load_si256(&input1);
    auto avx2 = _mm256_load_si256(&input2);
    auto avxr = _mm256_xor_si256(avx1, avx2);

    avx2_int32vec_t result;
    _mm256_store_si256(&result, avxr);
    trace("result", result);

    return 0;
}
