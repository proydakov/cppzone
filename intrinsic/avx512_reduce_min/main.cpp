#include "../avx512_vec.h"

int main()
{
    avx512_int32vec_t input1;

    std::cout << "Min in vector(int32_t)[" << avx512_int32vec_t::size() << "]\n";

    read("input1", input1);

    auto avx1 = _mm512_load_si512(&input1);
    auto result = _mm512_reduce_min_epi32(avx1);

    std::cout << "minimum: " << result << std::endl;

    return 0;
}
