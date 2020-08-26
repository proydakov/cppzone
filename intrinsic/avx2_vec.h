#include <immintrin.h>
#include "avxvec_io.h"

template<typename T>
struct alignas(32) avx2_t
{
    static constexpr size_t size() noexcept
    {
        return 32 / sizeof(T);
    }

    auto operator&()
    {
        if constexpr (std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int> || std::is_same_v<T, unsigned long>)
            return reinterpret_cast<__m256i*>(&val);
        else if constexpr (std::is_same_v<T, float>)
            return reinterpret_cast<__m256*>(&val);
        else if constexpr (std::is_same_v<T, double>)
            return reinterpret_cast<__m256d*>(&val);
        else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    }

    auto operator&() const
    {
        if constexpr (std::is_same_v<T, short> || std::is_same_v<T, int> || std::is_same_v<T, long> || std::is_same_v<T, unsigned short> || std::is_same_v<T, unsigned int> || std::is_same_v<T, unsigned long>)
            return reinterpret_cast<__m256i const*>(&val);
        else if constexpr (std::is_same_v<T, float>)
            return reinterpret_cast<__m256 const*>(&val);
        else if constexpr (std::is_same_v<T, double>)
            return reinterpret_cast<__m256d const*>(&val);
        else
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
    }

    T val[size()];

    static_assert(sizeof(val) == 32, "AVX2 require 32 byte alignment for data types.");
};

using avx2_int32vec_t = avx2_t<int>;
