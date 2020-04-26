#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(LIBRARY_FILES
    object.h
    object.cpp
    )

I_ADD_SHARED_LIBRARY(unix_object ${LIBRARY_FILES})

SET(SOURCE_FILES
    main.cpp
    )

I_ADD_EXECUTABLE(linux_dynamic_loading ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(linux_dynamic_loading ${DL_LIBRARY})
