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
    CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_LINK=1 -DCLANG_ROOT=/usr/lib/llvm-10 -DBOOST_ROOT=/home/proydakov/sdks/boost_1_65_0_libc++/ .. #CLANG_ROOT AND BOOST_ROOT require for libc++
    make -j #ninja

# build boost with libc++

    ./bootstrap.sh --with-toolset=clang
    ./b2 toolset=clang link=static cxxflags="-stdlib=libc++" linkflags="-stdlib=libc++" --prefix=/home/proydakov/sdks/boost_version_libc++ install
