#include <main.h>
#include <test_heap_data.h>
#include <one2many_bitmask_queue.h>

int main(int argc, char* argv[])
{
    return test_main<one2many_bitmask_queue<data_t, std::uint64_t>, perf_allocated_test>( argc, argv);
}
