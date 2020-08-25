#include <immintrin.h>
#include "avxvec_io.h"

template<typename Type>
struct alignas(32) avx2_t
{
    static constexpr size_t size() noexcept
    {
        return 32 / sizeof(Type);
    }

    __m256i* operator&()
    {
        return reinterpret_cast<__m256i*>(&val);
    }

    __m256i const* operator&() const
    {
	return reinterpret_cast<__m256i const*>(&val);
    }

    Type val[size()];

    static_assert(sizeof(val) == 32, "AVX2 require 32 byte alignment for data types.");
};

using avx2_int32vec_t = avx2_t<int>;
