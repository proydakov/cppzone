#include <cmath>
#include <chrono>
#include <iostream>
#include <unordered_map>

int main()
{
    std::chrono::high_resolution_clock clock;

    std::unordered_map<int, int> table;
    table.reserve(16);

    for(int i = 0; ; i++) {
        if( table.bucket_count() - 1 ==  table.size()) {
            auto start = clock.now();
            table.insert(std::make_pair(i, i));
            auto end = clock.now();
            std::cout << "bucket_count: " << table.bucket_count() << " time:    "
                      << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
                      << " microseconds" << std::endl;
        }
        else {
            table.insert(std::make_pair(i, i));
        }
    }
    return 0;
}
