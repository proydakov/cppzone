#include <iostream>

template<typename Type>
struct alignas(64) avx512_t
{
    static constexpr size_t size() noexcept
    {
        return 64 / sizeof(Type);
    }

    Type val[size()];

    static_assert(sizeof(val) == 64, "AVX512 require 64 byte alignment for data types.");
};

template<typename AVX512VEC>
void read(char const * const name, AVX512VEC& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

template<typename AVX512VEC>
void trace(char const * const name, AVX512VEC const& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec.val[i] << ' ';
    }
    std::cout << "\n";
}
