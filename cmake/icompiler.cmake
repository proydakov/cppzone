macro(COMPILER_SET_MAX_WARNING_LEVEL)

if(MSVC)
    string(REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
else()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -Wno-unknown-pragmas -Wno-noexcept-type")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-unknown-attributes -Wno-unknown-pragmas")
    endif()
endif()

endmacro(COMPILER_SET_MAX_WARNING_LEVEL)

###############################################################################

MACRO(COMPILER_MSVC_SET_STATIC_RUNTIME)

IF(NOT MSVC)
    MESSAGE(FATAL_ERROR "Invalid macro call MSVC_SET_STATIC_RUNTIME. The compiler does not meet the requirements.")
ENDIF(NOT MSVC)
    
#We statically link to reduce dependancies
FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    IF(${flag_var} MATCHES "/MD")
        STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    ENDIF(${flag_var} MATCHES "/MD")
    IF(${flag_var} MATCHES "/MDd")
        STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    ENDIF(${flag_var} MATCHES "/MDd")
ENDFOREACH(flag_var)

ENDMACRO(COMPILER_MSVC_SET_STATIC_RUNTIME)

###############################################################################

MACRO(SETUP_CXX_FLAGS)

#set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS 1) # https://cmake.org/cmake/help/latest/prop_tgt/CXX_EXTENSIONS.html#prop_tgt:CXX_EXTENSIONS
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(PROJECT_OS_LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-new")
        endif()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif()
endif()

if(PROJECT_OS_OSX)
    set(CMAKE_MACOSX_RPATH 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY           "libc++")
endif()

if(MSVC)
    ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
    ADD_DEFINITIONS(-D_DISABLE_EXTENDED_ALIGNED_STORAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
#else()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -Wnon-virtual-dtor")
endif()

COMPILER_SET_MAX_WARNING_LEVEL()
#COMPILER_MSVC_SET_STATIC_RUNTIME()

ENDMACRO(SETUP_CXX_FLAGS)
