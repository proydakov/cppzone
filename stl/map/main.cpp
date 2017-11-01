#include <map>
#include <vector>
#include <iostream>

typedef std::map<int, int> map_t;

int main()
{
    map_t map{
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 4, 4 },
        { 5, 5 }
    };

    std::vector<map_t::const_iterator> vec;
    vec.push_back(map.find(1));
    vec.push_back(map.find(2));
    vec.push_back(map.find(3));

    std::cout << "b1" << std::endl;
    for(const auto& it : vec) {
        std::cout << (it->second) << std::endl;
    }

    for(int i = -10; i < 0; i++) {
        map.emplace(i, i);
    }

    for(int i = 9; i < 20; i++) {
        map.emplace(i, i);
    }

    std::cout << "b2" << std::endl;
    for(const auto& it : vec) {
        std::cout << (it->second) << std::endl;
    }

    return 0;
}
