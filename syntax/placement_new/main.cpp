#include <memory>
#include <iostream>

struct alignas(32) data_t
{
    data_t(void* ptr) : ptr_(ptr)
    {
        std::cout << "align ok: " << (((intptr_t)this % alignof(data_t)) == 0) << std::endl;
    }

    void* ptr_;
};

void process(int i)
{
    if(i < 1) {
        return;
    }

    char mem[sizeof(data_t) * 2];
    data_t *obj = new (mem) data_t(nullptr);
    std::cout << "eq: " << ((void*)&mem == (void*)obj) << std::endl;  

    process(i - 1);
}

int main(int argc, char* argv[])
{
    int iters = 10;
    if(2 == argc) {
        iters = std::stoi(argv[1]);
    }

    process(iters);

    return 0;
}
