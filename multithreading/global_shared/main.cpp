#include <mutex>
#include <memory>
#include <thread>
#include <iostream>

constexpr std::size_t PTR_SIZE = sizeof(void*);
constexpr std::size_t SHARED_PTR_ALIGN = PTR_SIZE * 2;

struct alignas(SHARED_PTR_ALIGN) shared
{
    std::shared_ptr<int> val;
};

shared g_ptr;

int main()
{
    std::ios::sync_with_stdio(false);

    void* addr = &g_ptr;
    std::cout << "addr ok: " << ((std::size_t)(addr) % SHARED_PTR_ALIGN == 0) << std::endl;

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
