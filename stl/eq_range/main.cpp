#include <vector>
#include <iostream>
#include <algorithm>

int main ()
{
    std::vector<int> v{10, 20, 30, 30, 20, 10, 10, 20};

    std::sort (v.begin(), v.end());                   // 10 10 10 20 20 20 30 30

    std::vector<int>::iterator low,up;
    low = std::lower_bound (v.begin(), v.end(), 19); //            ^
    up  = std::upper_bound (v.begin(), v.end(), 20); //                     ^

    std::cout << "lower_bound at position " << (low - v.begin()) << std::endl;
    std::cout << "upper_bound at position " << (up  - v.begin()) << std::endl;

    return 0;
}
