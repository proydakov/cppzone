#include <cstring>
#include <cstdint>
#include <iostream>

union data_t
{
    uint32_t num;
    char     arr[4];
};

int main()
{
    data_t data;
    memset(&data, 0, sizeof(data_t));
    data.arr[0] = 1;

    std::cout << "num: " << data.num << std::endl;

    return 0;
}
