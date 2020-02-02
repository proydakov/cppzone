#include <vector>
#include <cstdint>
#include <iomanip>
#include <iostream>

struct version1;
struct version2;

template<class version_t, bool trace>
class parser
{
public:
    std::size_t parse(const std::vector<char>& buffer)
    {
        std::cout << "general parse" << std::endl;
        return parseImpl(buffer);
    }

private:
    std::size_t parseImpl(const std::vector<char>& buffer)
    {
        std::size_t result = 0;
        for (auto it = buffer.begin(), end_it = buffer.end(); it != end_it; ++it)
        {
            result ^= static_cast<std::size_t>(*it);
            if (trace)
            {
                std::cout << __FILE__ << ':' << __LINE__ << " byte: " << *it << std::endl;
            }
        }
        return result;
    }
};

std::size_t t1()
{
    parser<version1, true> p1;
    std::vector<char> buffer{ 'a', 'b', 'c' };
    return p1.parse(buffer);
}

std::size_t t2()
{
    parser<version2, false> p2;
    std::vector<char> buffer{ 'a', 'b', 'c' };
    return p2.parse(buffer);
}

int main()
{
    std::size_t res = t1() ^ t2();
    std::cout << "\nres: " << res << std::endl;
    int i = 0;
    std::cin >> i;
    return 0;
}
