#include <string>
#include <vector>
#include <iostream>

size_t allocated = 0;
size_t obj_size = 0;
size_t allocated_size = 0;

void* operator new(size_t sz) throw (std::bad_alloc)
{
    if(sz != obj_size) {
        obj_size = sz;
        std::cout << "size: " << sz << std::endl;
    }

    void* mem = malloc(sz);
    if (nullptr == mem) {
        throw std::bad_alloc();
    }
    size_t* ptr = (size_t*)(mem);
    ptr -= 1;

    const size_t allocated_step = *ptr;
    allocated += allocated_step;

    if(allocated_step != allocated_size) {
        allocated_size = allocated_step;
        std::cout << "allocated_size: " << allocated_size << std::endl;
    }
    return mem;
}

void operator delete(void* ptr) throw()
{
    free(ptr);
}

int main(int argc, char **argv)
{
    std::string data{"ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890 0987654321 1234567890"};

    std::vector<std::string> vect(1024 * 1024 * 32, data);

    std::cout << "sizeof str: " << sizeof(std::string) << std::endl;
    std::cout << "size data:  " << data.capacity() + 1 << std::endl;

    if(allocated > 1024 * 1024 * 1024) {
        std::cout << "allocated:  " << 1.0 * allocated / (1024 * 1024 * 1024) << " Gb\n";
    }
    else {
        std::cout << "allocated:  " << 1.0 * allocated << std::endl;
    }
    std::cout << std::endl;

    size_t i = 0;
    std::cin >> i;

    return 0;
}
