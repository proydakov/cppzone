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

macro(SETUP_LINKER)
    string(REPLACE "." ";" COMPILER_VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
    list(GET COMPILER_VERSION_LIST 0 MAJOR_CXX_VERSION)

    if(UNIX)
        find_program(LLD_PATH NAMES "lld-${MAJOR_CXX_VERSION}" "lld")
        find_program(GOLD_PATH NAMES "gold")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # by default gcc and clang use stdc++
        set(CXX_RUNTIME_LIBRARY "libstdc++")
        # try to use gold instead of ld
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND GOLD_PATH)
            set(LINKER_NAME "gold")
            set(CMAKE_LINKER ${GOLD_PATH})
        endif()

        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND LLD_PATH)
            set(LINKER_NAME "lld")
            set(CMAKE_LINKER ${LLD_PATH})
        endif()

        if(STRIP_LINK)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
        endif()

        if(STATIC_LINK)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared")
        endif()
    elseif(MSVC)
        set(CXX_RUNTIME_LIBRARY "msvc")

        if(STATIC_LINK)
            COMPILER_MSVC_SET_STATIC_RUNTIME()
        endif()
    else()
        set(CXX_RUNTIME_LIBRARY "unknown")
    endif()

    # try to use libc++ runtime for clang
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        execute_process(COMMAND readlink -fn ${CMAKE_C_COMPILER} OUTPUT_VARIABLE TEST_CLANG_ROOT)
        string(REPLACE "/bin/clang" "" TEST_CLANG_ROOT ${TEST_CLANG_ROOT})

        if(EXISTS "${TEST_CLANG_ROOT}/include/c++/v1/" AND EXISTS "${TEST_CLANG_ROOT}/lib/libc++.a" AND EXISTS "${TEST_CLANG_ROOT}/lib/libc++abi.a")
            message(STATUS "Detected clang root: ${TEST_CLANG_ROOT}")
            set(CLANG_ROOT ${TEST_CLANG_ROOT})
        endif()

        # try to use libc++
        if (CLANG_ROOT)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -nostdinc++ -I${CLANG_ROOT}/include/c++/v1")
            set(CLANG_EXTRA_LINKS "-stdlib=libc++ -lc++abi -L${CLANG_ROOT}/lib -Wl,-rpath,${CLANG_ROOT}/lib")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CLANG_EXTRA_LINKS}")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CLANG_EXTRA_LINKS}")
            set(CXX_RUNTIME_LIBRARY "libc++")
        endif ()
    endif()

    if(LINKER_NAME)
        message(STATUS "Using linker: ${LINKER_NAME} (selected from: LLD_PATH=${LLD_PATH}; GOLD_PATH=${GOLD_PATH})")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=${LINKER_NAME}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=${LINKER_NAME}")
    else()
        message(STATUS "Use default linker")
    endif()

    # https://stackoverflow.com/questions/35116327/when-g-static-link-pthread-cause-segmentation-fault-why
    if(STATIC_LINK AND CXX_RUNTIME_LIBRARY STREQUAL "libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
    endif()

    if(STATIC_LINK AND CXX_RUNTIME_LIBRARY STREQUAL "libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
    endif()

    if(UNIX AND NOT STATIC_LINK)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
    endif()

endmacro(SETUP_LINKER)
