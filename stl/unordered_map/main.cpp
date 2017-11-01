#include <vector>
#include <iostream>
#include <unordered_map>

using map_t = std::unordered_map<int, int>;

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

    std::cout << "bucket_count 1: " << map.bucket_count() << std::endl;

    for(int i = -100; i < 0; i++) {
        map.emplace(i, i);
    }

    for(int i = 9; i < 200; i++) {
        map.emplace(i, i);
    }

    std::cout << "bucket_count 2: " << map.bucket_count() << std::endl;

    std::cout << "b2" << std::endl;
    for(const auto& it : vec) {
        std::cout << (it->second) << std::endl;
    }

    return 0;
}
