#include <map>
#include <string>
#include <vector>
#include <iostream>

int main()
{
    using map_t = std::map<int, std::string>;

    map_t map{
        { 1, "v1" },
        { 2, "v2" },
        { 3, "v3" },
        { 4, "v4" },
        { 5, "v5" }
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
        std::string str("v");
        str += std::to_string(i);
        map.emplace(i, str);
    }

    for(int i = 9; i < 20; i++) {
        std::string str("v");
        str += std::to_string(i);
        map.emplace(i, str);
    }

    std::cout << "b2" << std::endl;
    for(const auto& it : vec) {
        std::cout << (it->second) << std::endl;
    }

    return 0;
}
