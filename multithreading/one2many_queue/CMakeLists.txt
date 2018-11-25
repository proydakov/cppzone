#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

I_ADD_EXECUTABLE(multithreading_one2many_bitmask_queue_perf_plain
    test_main.h
    test_plain_data.h
    one2many_bitmask_queue.h
    test_bitmask_queue_plain_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_bitmask_queue_perf_heap
    test_main.h
    test_heap_data.h
    one2many_bitmask_queue.h
    test_bitmask_queue_heap_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_bitmask_queue_latency
    test_main.h
    test_latency.h
    one2many_bitmask_queue.h
    test_bitmask_queue_latency.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_counter_queue_perf_plain
    test_main.h
    test_plain_data.h
    one2many_counter_queue.h
    test_counter_queue_plain_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_counter_queue_perf_heap
    test_main.h
    test_heap_data.h
    one2many_counter_queue.h
    test_counter_queue_heap_data.cpp
)
