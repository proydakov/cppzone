#include <cstdio>
#include <chrono>
#include <vector>
#include <iostream>

#if _MSC_VER

#include <Windows.h>

int getpagesize()
{
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return systemInfo.dwPageSize;
}

#else

#include <unistd.h>

#endif

const int KB = 1024;
const int MB = 1024 * KB;
const int data_size = 128 * MB / sizeof(int);
const int repeats = 64 * MB;
const int steps = 8 * MB;
const int times = 8;
const float cache_k = 1.15;

long long clock_time()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return millis;
}

int main()
{
    int* data = new int[data_size];
    for (int i = 0; i < data_size; i++) {
        data[i]++;
    }

    std::vector<int> steps({
        1 * KB,   2 * KB,   4 * KB,  8 * KB,
        16 * KB,  32 * KB,  64 * KB, 128 * KB,
        256 * KB, 512 * KB, 1 * MB,  2 * MB,
        3 * MB,   4 * MB,   5 * MB,  6 * MB,
        7 * MB,   8 * MB,   9 * MB,  10 * MB,
        11 * MB,  12 * MB,  13 * MB, 14 * MB,
        15 * MB,  16 * MB,  24 * MB, 32 * MB
    });

    std::vector< std::pair<int, double> > labs;
    for (size_t i = 0; i < steps.size(); i++) {
        double totalTime = 0;    
        int size_mask = steps[i] / sizeof(int) - 1;

        for (int k = 0; k < times; k++) {
            long long start = clock_time();
            for (int j = 0; j < repeats; j++) {
                ++data[ (j * sizeof(int) * sizeof(int) * sizeof(int) ) & size_mask ];
            }
            long long end = clock_time();
            totalTime += (end - start) / 1000000000.0;
        }
        labs.push_back( std::make_pair(steps[i] / KB, totalTime) );
        printf("%d time: %lf\n", steps[i] / KB, totalTime);
    }

    int calc_cache = 0;
    double current = labs[0].second;
    for(size_t i = 1; i < labs.size() - 1; i++) {
        if(labs[i].second > current * cache_k &&
            labs[i].second > labs[i + 1].second * cache_k) {
            calc_cache = labs[i].first;
            break;
        }
        current = labs[i].second;
    }
    printf("calc   cache: %d\n", calc_cache);
    printf("system cache: %d\n", getpagesize());

    delete[] data;
    return 0;
}
