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
    data.arr[0] = 1;

    std::cout << "num: " << data.num << std::endl;

    return 0;
}
