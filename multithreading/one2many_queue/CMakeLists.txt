#==============================================================================
# Copyright (c) 2018 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


I_ADD_EXECUTABLE(multithreading_one2many_perf_plain
    one2many_queue.h
    performance.h
    performance_plain_data.cpp
)

I_ADD_EXECUTABLE(multithreading_one2many_perf_allocated
    one2many_queue.h
    performance.h
    performance_allocated_data.cpp
)
