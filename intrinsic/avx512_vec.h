#include <immintrin.h>
#include "avxvec_io.h"

template<typename T>
struct alignas(64) avx512_t
{
    static constexpr size_t size() noexcept
    {
        return 64 / sizeof(T);
    }

    auto operator&()
    {
        if constexpr (std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int> || std::is_same_v<T, unsigned long>)
            return reinterpret_cast<__m512i*>(&val);
        else if constexpr (std::is_same_v<T, float>)
            return reinterpret_cast<__m512*>(&val);
        else if constexpr (std::is_same_v<T, double>)
            return reinterpret_cast<__m512d*>(&val);
        else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    }

    auto operator&() const
    {
        if constexpr (std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int> || std::is_same_v<T, unsigned long>)
            return reinterpret_cast<__m512i const*>(&val);
        else if constexpr (std::is_same_v<T, float>)
            return reinterpret_cast<__m512 const*>(&val);
        else if constexpr (std::is_same_v<T, double>)
            return reinterpret_cast<__m512d const*>(&val);
        else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    }

    T val[size()];

    static_assert(sizeof(val) == 64, "AVX512 require 64 byte alignment for data types.");
};

using avx512_int32vec_t = avx512_t<int>;
