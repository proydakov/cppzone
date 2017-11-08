#include <mutex>
#include <thread>
#include <iostream>

struct alignas(16) shared
{
    std::shared_ptr<int> val;
};

shared g_ptr;

int main()
{
    std::ios::sync_with_stdio(false);

    void* addr = &g_ptr;
    std::cout << "addr ok: " << ((intptr_t)addr % 16 == 0) << std::endl;

    std::thread t1([](){
        while(true) {
            auto ptr = std::make_shared<int>(1);
            g_ptr.val = ptr;
        }
    });

    std::thread t2([](){
        while(true) {
            auto ptr = g_ptr.val;
            std::cout << "ptr: " << *ptr << std::endl;
        }
    });

    t1.join();
    t2.join();

    return 0;
}
