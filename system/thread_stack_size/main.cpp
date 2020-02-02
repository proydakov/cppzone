#include <thread>
#include <vector>
#include <iostream>

int main(int, char*[])
{
    const size_t size = 4096 - 1;
    std::vector<std::thread> threads;
    threads.reserve(size);

    for(size_t i = 0; i < size; i++) {
        std::thread thread([](){
            std::this_thread::sleep_for (std::chrono::seconds(1 * 60 * 60));
        });
        thread.detach();
        threads.push_back(std::move(thread));
    }

    int i = 0;
    std::cout << "wait input..." << std::endl;
    std::cin >> i;

    return 0;
}
