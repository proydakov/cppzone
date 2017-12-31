#include <cstdint>
#include <iomanip>
#include <iostream>

// fibonacci_positive

template<int64_t N>
struct fibonacci_positive
{
    enum int64_t { value = fibonacci_positive<N - 1>::value + fibonacci_positive<N - 2>::value };
};

template<>
struct fibonacci_positive<0>
{
    enum int64_t { value = 0 };
};

template<>
struct fibonacci_positive<1>
{
    enum int64_t { value = 1 };
};

template<>
struct fibonacci_positive<2>
{
    enum int64_t { value = 1 };
};

// fibonacci_negative

template<int64_t N>
struct fibonacci_negative
{
    enum int64_t { value = fibonacci_negative<N + 2>::value - fibonacci_negative<N + 1>::value };
};

template<>
struct fibonacci_negative<0>
{
    enum int64_t { value = 0 };
};

template<>
struct fibonacci_negative<1>
{
    enum int64_t { value = 1 };
};

template<>
struct fibonacci_negative<2>
{
    enum int64_t { value = 1 };
};

// fibonacci

template<int64_t N>
struct fibonacci
{
    typedef typename std::conditional<N >= 0, fibonacci_positive<N>, fibonacci_negative<N>>::type backend;
};

template<int64_t N>
void test()
{
    auto value = fibonacci<N>::backend::value;
    std::cout << "fibonacci(" << std::setw(5) << N << "): " << std::setw(10) << value << std::endl;
}

int main()
{
    std::ios::sync_with_stdio(false);

    test<0>();
    test<1>();
    test<2>();
    test<15>();
    test<-8>();

    return 0;
}
