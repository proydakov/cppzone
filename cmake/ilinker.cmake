macro(SETUP_LINKER)

string(REGEX MATCH "-?[0-9]+(.[0-9]+)?$" COMPILER_POSTFIX ${CMAKE_CXX_COMPILER})

find_program (LLD_PATH NAMES "lld${COMPILER_POSTFIX}" "lld")
find_program (GOLD_PATH NAMES "gold")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND LLD_PATH AND CLANG_ROOT AND NOT LINKER_NAME)
    set (LINKER_NAME "lld")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${CLANG_ROOT}/lib")
#    link_libraries (-stdlib=libc++ c++ c++abi)
elseif (GOLD_PATH)
    set (LINKER_NAME "gold")
endif ()

if (LINKER_NAME)
    message(STATUS "Using linker: ${LINKER_NAME} (selected from: LLD_PATH=${LLD_PATH}; GOLD_PATH=${GOLD_PATH}; COMPILER_POSTFIX=${COMPILER_POSTFIX})")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=${LINKER_NAME}")
else ()
    message("Use default linker")
endif ()

endmacro(SETUP_LINKER)
