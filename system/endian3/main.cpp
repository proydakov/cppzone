#include <cstdint>
#include <iostream>

union data_t
{
    int64_t num;
    char    arr[8];
};

int main()
{
    data_t data;
    data.num = -7;

    std::cout << "data: \n" << data.num << "\n";
    for(int i = 0; i < 8; i++) {
        std::cout << (int)data.arr[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}
