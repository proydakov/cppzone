# cppzone

[![Travis CI Status](https://travis-ci.org/proydakov/cpplabs.svg?branch=master)](https://travis-ci.org/proydakov/cpplabs)
[![Appveyor CI status](https://ci.appveyor.com/api/projects/status/github/proydakov/cpplabs?svg=true)](https://ci.appveyor.com/project/proydakov/cpplabs)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/proydakov/cpplabs/blob/master/COPYING)

My ASM/C/C++, Lua, OpenGL, Direct3D experiments for desktop computer. Windows, MacOSX and Linux.

# build gcc

    mkdir build-gcc
    cd build-gcc
    cmake ..
    make -j #ninja

# build clang

    mkdir build-clang
    cd build-clang
    CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCLANG_ROOT=/opt/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04 .. #CLANG_ROOT require for libc++
    make -j #ninja
