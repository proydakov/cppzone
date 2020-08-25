#include <iostream>

template<typename AVXVEC>
void read(char const * const name, AVXVEC& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cin >> vec.val[i];
    }
}

template<typename AVXVEC>
void trace(char const * const name, AVXVEC const& vec)
{
    std::cout << name << ": ";
    for(size_t i = 0; i < vec.size(); i++)
    {
        std::cout << vec.val[i] << ' ';
    }
    std::cout << "\n";
}
