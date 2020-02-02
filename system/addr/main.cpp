#include <cstdint>
#include <iostream>

char const* const g_p_str = "Hello, x64 addr.";

// https://en.wikipedia.org/wiki/Tagged_pointer
// https://compas.cs.stonybrook.edu/~nhonarmand/courses/fa17/cse306/slides/07-x86_vm.pdf
template<typename T, int virtual_addresses_bitcount = 48>
struct tagged_ptr
{
    static constexpr std::uint64_t vaddr_bits = ((1ul << virtual_addresses_bitcount) - 1);

    tagged_ptr(T* ptr) : m_data(std::uint64_t(ptr)) {}

    void operator|=(std::uint64_t mask)
    {
	    m_data |= mask & ~vaddr_bits;
    }

    void operator&=(std::uint64_t mask)
    {
        m_data &= mask & ~vaddr_bits;
    }

    void operator^=(std::uint64_t mask)
    {
        m_data ^= mask & ~vaddr_bits;
    }

    T* ptr_data()
    {
        return (T*)(m_data & vaddr_bits);
    }

    std::uint64_t mask_data()
    {
        return m_data & ~vaddr_bits;
    }

    std::uint64_t raw_data()
    {
        return m_data;
    }

private:
    static_assert(sizeof(std::uint64_t) == sizeof(T*));
    std::uint64_t m_data;
};

int main()
{
    char const* p_str = g_p_str;
    std::cout << "addr: " << (void*)(p_str) << std::flush;
    std::cout << " raw: " << std::hex << (std::uint64_t)(p_str) << std::flush;
    std::cout << " value: " << p_str << std::endl;

    tagged_ptr<char const> p64(g_p_str);

    p64 |= ~0ul;

    std::cout << "addr: " << (void*)(p64.ptr_data()) << std::flush;
    std::cout << " mask: " << std::hex << p64.mask_data() << std::flush;
    std::cout << " raw: " << std::hex << (std::uint64_t)(p64.raw_data()) << std::flush;
    std::cout << " value: " << p64.ptr_data() << std::endl;

    return 0;
}
