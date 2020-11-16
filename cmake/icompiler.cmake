macro(COMPILER_SET_MAX_WARNING_LEVEL)
    if(MSVC)
        string(REGEX REPLACE /W[0-4] "/W4 /WX" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
        string(REGEX REPLACE /W[0-4] "/W4 /WX" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    else()
        set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors -Wunused -Wconversion -Wsign-conversion -fstrict-aliasing")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -pedantic-errors -Wunused -Wconversion -Wsign-conversion -fstrict-aliasing")

        if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-attributes -Wno-unknown-pragmas")
        endif()
endif()

endmacro(COMPILER_SET_MAX_WARNING_LEVEL)

###############################################################################

MACRO(SETUP_COMPILER_FLAGS)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_EXTENSIONS 0)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if(PROJECT_OS_LINUX)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        endif()

        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        endif()
    endif()

    if(PROJECT_OS_OSX)
        set(CMAKE_MACOSX_RPATH 1)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17")
        set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY           "libc++")
    endif()

    if(MSVC)
        ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
        ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
        ADD_DEFINITIONS(-D_DISABLE_EXTENDED_ALIGNED_STORAGE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
    endif()

    COMPILER_SET_MAX_WARNING_LEVEL()
ENDMACRO(SETUP_COMPILER_FLAGS)

###############################################################################

MACRO(SETUP_TOOL_FLAGS)
    # colors =)
    set (CMAKE_BUILD_COLOR_MAKEFILE ON)

    if(CMAKE_GENERATOR STREQUAL "Ninja")
        # Turn on colored output. https://github.com/ninja-build/ninja/wiki/FAQ
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics")
            set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
        endif()
    endif()
ENDMACRO(SETUP_TOOL_FLAGS)
