#==============================================================================
# Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

FIND_PACKAGE(Boost)
INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})

SET(SOURCE_FILES main.cpp)

I_ADD_EXECUTABLE(boost_interrupt ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(boost_interrupt ${Boost_SYSTEM_LIBRARY})
TARGET_LINK_LIBRARIES(boost_interrupt ${Boost_THREAD_LIBRARY})
TARGET_LINK_LIBRARIES(boost_interrupt ${Boost_DATE_TIME_LIBRARY})
TARGET_LINK_LIBRARIES(boost_interrupt ${Boost_REGEX_LIBRARY})
TARGET_LINK_LIBRARIES(boost_interrupt ${Boost_CHRONO_LIBRARY})

if(MSVC)
    TARGET_COMPILE_OPTIONS(boost_interrupt PRIVATE "/wd4244")
endif()
