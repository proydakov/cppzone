#include <string>
#include <vector>
#include <iostream>

size_t allocated = 0;
size_t obj_size = 0;
size_t allocated_size = 0;

void* operator new(size_t sz)
{
    if(sz != obj_size) {
        obj_size = sz;
        //std::cout << "size: " << sz << std::endl;
    }

    void* mem = malloc(sz);
    if (nullptr == mem) {
        throw std::bad_alloc();
    }

#ifdef _MSC_VER 
    const size_t allocated_step = sz + 16 + 3;
#else
    volatile size_t* ptr = (volatile size_t*)(mem);
    ptr -= 1;
    size_t allocated_step =  *ptr;
#endif

    allocated += allocated_step;

    if(allocated_step != allocated_size) {
        allocated_size = allocated_step;
        //std::cout << "allocated_size: " << allocated_size << std::endl;
    }
    return mem;
}

void operator delete(void* ptr) noexcept
{
    free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
    free(ptr);
}

int main(int, char*[])
{
    std::string data{"ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890 0987654321 1234567890"};

    std::vector<std::string> vect(1024 * 1024 * 16, data);

    std::cout << "sizeof str: " << sizeof(std::string) << std::endl;
    std::cout << "size data:  " << data.capacity() + 1 << std::endl;

    if(allocated > 1024 * 1024 * 1024)
    {
        std::cout << "allocated:  " << double(allocated) / (1024 * 1024 * 1024) << " Gb\n";
    }
    else
    {
        std::cout << "allocated:  " << double(allocated) << std::endl;
    }
    std::cout << std::endl;

    size_t i = 0;
    std::cin >> i;

    return 0;
}
