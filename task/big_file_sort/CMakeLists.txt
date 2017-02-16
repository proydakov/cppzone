#==============================================================================
# Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

I_ADD_EXECUTABLE(task_big_file_sort_memlimit main_sort_memlimit.cpp)
I_ADD_EXECUTABLE(task_big_file_sort_std main_sort_std.cpp)
I_ADD_EXECUTABLE(task_big_file_generate main_generate.cpp)
I_ADD_EXECUTABLE(task_big_file_validate main_validate.cpp)
I_ADD_EXECUTABLE(task_big_file_read main_read.cpp)
