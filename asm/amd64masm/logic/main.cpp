#include <vector>
#include <string>
#include <iostream>

extern "C" bool LogicAND(bool i1, bool i2);
extern "C" bool LogicOR (bool i1, bool i2);
extern "C" bool LogicXOR(bool i1, bool i2);

template<class T>
void test(T func, const std::string& desc)
{
    std::vector<bool> vec{false, true};

    for (size_t i = 0; i < vec.size(); i++) {
        for (size_t j = 0; j < vec.size(); j++) {
            std::cout << "test func: " << desc << " for: " << vec[i] << " " << vec[j] << " res: " << func(vec[i], vec[j]) << std::endl;
        }
    }
    std::cout << "" << std::endl;
}

int main()
{
    test(LogicAND, "LogicAND");
    test(LogicOR,  "LogicOR");
    test(LogicXOR, "LogicXOR");

    return 0;
}
