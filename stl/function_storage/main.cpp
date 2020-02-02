#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <functional>

void* operator new(std::size_t sz)
{
    std::cout << "malloc, size=" << sz;
    auto ptr = std::malloc(sz);
    std::cout << " ptr=" << ptr << std::endl;
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    std::cout << "free, ptr=" << ptr << std::endl;
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    std::cout << "free, ptr=" << ptr << " size: " << size << std::endl;
    std::free(ptr);
}

template<std::size_t N>
void test()
{
    std::cout << "----- test[" << N << "] -----" << std::endl;

    struct plain_t
    {
        std::byte data[N];
    };

    plain_t plain{};

    std::function<void()> function = [plain]() mutable
    {
        memset(&plain.data, 0, sizeof(plain.data));
    };

    std::cout << "sizeof(plain): " << sizeof(plain)
        << " sizeof(function): " << sizeof(function)
        << std::endl;
}

int main()
{
    test<1>();
    test<2>();
    test<3>();
    test<4>();
    test<5>();
    test<6>();
    test<7>();
    test<8>();
    test<9>();
    test<10>();
    test<11>();
    test<12>();
    test<13>();
    test<14>();
    test<15>();
    test<16>();
    test<17>();
    test<18>();
    test<19>();
    test<20>();
    test<21>();
    test<22>();
    test<23>();
    test<24>();
    test<25>();
    test<26>();
    test<27>();
    test<28>();
    test<29>();
    test<30>();
    test<31>();
    test<32>();
    test<33>();
    test<34>();
    test<35>();
    test<36>();
    test<37>();
    test<38>();
    test<39>();
    test<40>();
    test<41>();
    test<42>();
    test<43>();
    test<44>();
    test<45>();
    test<46>();
    test<47>();
    test<48>();
    test<49>();
    test<50>();
    test<51>();
    test<52>();
    test<53>();
    test<54>();
    test<55>();
    test<56>();
    test<57>();
    test<58>();
    test<59>();
    test<60>();
    test<61>();
    test<62>();
    test<63>();
    test<64>();

    return 0;
}
