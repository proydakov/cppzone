#include <iostream>

template<typename Type>
struct alignas(32) avx2_t
{
    static constexpr size_t size() noexcept
    {
        return 32 / sizeof(Type);
    }

    Type val[size()];

    static_assert(sizeof(val) == 32, "AVX2 require 32 byte alignment for data types.");
};

template<typename AVX2VEC>
void read(char const * const name, AVX2VEC& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

template<typename AVX2VEC>
void trace(char const * const name, AVX2VEC const& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec.val[i] << ' ';
    }
    std::cout << "\n";
}
