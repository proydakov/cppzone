#include "test_main.h"
#include "test_allocated_data.h"

#include <one2many_bitmask_queue.h>

int main(int argc, char* argv[])
{
    perf_allocated_test test;
    return test_main<one2many_bitmask_queue<data_t>, perf_allocated_test>(test, argc, argv);
}
