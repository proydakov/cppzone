cmake -G Ninja -DCMAKE_C_COMPILER=clang \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DLIBCXX_HAS_MUSL_LIBC=ON \
        -DLLVM_PARALLEL_LINK_JOBS=4 \
        -DLLVM_ENABLE_PROJECTS="libcxx;libcxxabi;libunwind;clang;lldb;lld" \
        -DCMAKE_INSTALL_PREFIX=/home/proydakov/sdks/llvm-musl \
        ../llvm

