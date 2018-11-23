#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

I_ADD_EXECUTABLE(multithreading_one2many_bitmask_perf_plain
    test_main.h
    test_plain_data.h
    one2many_bitmask_queue.h
    performance_bitmask_plain_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_bitmask_perf_allocated
    test_main.h
    test_allocated_data.h
    one2many_bitmask_queue.h
    performance_bitmask_allocated_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_counter_perf_plain
    test_main.h
    test_plain_data.h
    one2many_counter_queue.h
    performance_counter_plain_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_counter_perf_allocated
    test_main.h
    test_allocated_data.h
    one2many_counter_queue.h
    performance_counter_allocated_data.cpp
)
