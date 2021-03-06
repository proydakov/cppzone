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

        if(STATIC_LINK)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared")
        endif()

        if(STRIP_LINK)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
        endif()
    elseif(APPLE)
        set(CXX_RUNTIME_LIBRARY "libc++")

        if(LLD_PATH)
            set(LINKER_NAME "lld")
            set(CMAKE_LINKER ${LLD_PATH})
        endif()

        if(STATIC_LINK)
            message(FATAL_ERROR "Apple doesn't support static link")
        endif()

        if(STRIP_LINK)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
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
        get_filename_component(TEST_CLANG_ROOT ${TEST_CLANG_ROOT} DIRECTORY)
        get_filename_component(TEST_CLANG_ROOT ${TEST_CLANG_ROOT} DIRECTORY)

        if(EXISTS "${TEST_CLANG_ROOT}/include/c++/v1/" AND EXISTS "${TEST_CLANG_ROOT}/lib/libc++.a" AND EXISTS "${TEST_CLANG_ROOT}/lib/libc++abi.a")
            message(STATUS "Detected clang root: ${TEST_CLANG_ROOT}")
            set(CLANG_ROOT ${TEST_CLANG_ROOT})
        endif()

        # try to use libc++
        if (CLANG_ROOT)
            add_compile_options(-nostdinc++ -I${CLANG_ROOT}/include/c++/v1)
            #add_compile_options(--sysroot ${MUSL_ROOT} -nostdinc -I${MUSL_ROOT}/include -nostdinc++ -I${CLANG_ROOT}/include/c++/v1)
            if(STATIC_LINK)
                set(CLANG_EXTRA_LINKS "-stdlib=libc++ -lpthread -lc++abi -L${CLANG_ROOT}/lib")
                # manual link list for static glibc & libc++ & libc++abi
                #set(CLANG_EXTRA_LINKS "-nodefaultlibs -lpthread -lc -lm -lgcc -lgcc_eh -lc++ -lc++abi -L${CLANG_ROOT}/lib")
                # manual link list for static musl & libc++ & libc++abi
                #set(CLANG_EXTRA_LINKS "-nodefaultlibs --sysroot ${MUSL_ROOT} -stdlib=libc++ -lc -lc++ -lc++abi -lunwind -L${CLANG_ROOT}/lib")
            else()
                set(CLANG_EXTRA_LINKS "-stdlib=libc++ -lpthread -lc++abi -L${CLANG_ROOT}/lib -Wl,-rpath,${CLANG_ROOT}/lib")
            endif()
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CLANG_EXTRA_LINKS}")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CLANG_EXTRA_LINKS}")
            set(CXX_RUNTIME_LIBRARY "libc++")
        endif ()
    endif()

    if(CXX_RUNTIME_LIBRARY STREQUAL "libstdc++")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=1")
    endif()

    if(STATIC_LINK AND CXX_RUNTIME_LIBRARY STREQUAL "libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
    elseif(CXX_RUNTIME_LIBRARY STREQUAL "libstdc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lpthread")
    endif()

    if(LINKER_NAME)
        message(STATUS "Using linker: ${LINKER_NAME} (selected from: LLD_PATH=${LLD_PATH}; GOLD_PATH=${GOLD_PATH})")
        set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=${LINKER_NAME}${CMAKE_EXE_LINKER_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=${LINKER_NAME}${CMAKE_SHARED_LINKER_FLAGS}")
    else()
        message(STATUS "Use default linker")
    endif()

endmacro(SETUP_LINKER)
