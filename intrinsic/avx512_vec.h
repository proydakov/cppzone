#include <immintrin.h>
#include "avxvec_io.h"

template<typename Type>
struct alignas(64) avx512_t
{
    static constexpr size_t size() noexcept
    {
        return 64 / sizeof(Type);
    }

    __m512i* operator&()
    {
        return reinterpret_cast<__m512i*>(&val);
    }

    __m512i const* operator&() const
    {
        return reinterpret_cast<__m512i const*>(&val);
    }

    Type val[size()];

    static_assert(sizeof(val) == 64, "AVX512 require 64 byte alignment for data types.");
};

using avx512_int32vec_t = avx512_t<int>;
