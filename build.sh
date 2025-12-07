#!/usr/bin/bash
set -e

[ -d build ] || mkdir build

[ -f libunwind/src/.libs/libunwind-x86_64.a ] && [ -f libunwind/src/.libs/libunwind.a ] || {
    cd libunwind

    # Build libunwind as per instructions from their README
    echo -e '\x1b[32mBuilding libunwind\x1b[0m'

    autoreconf -i
    CC=musl-gcc CFLAGS="-O2" ./configure --enable-shared=no --enable-debug=no --prefix=$PWD/../build
    make -j8

    echo -e '\x1b[32mDone building libunwind\x1b[0m'
    cd ..
}

# Unpack the libraries for repack
rm -rf unpacked
mkdir unpacked
cd unpacked

ar xf ../libunwind/src/.libs/libunwind-x86_64.a
ar xf ../libunwind/src/.libs/libunwind.a
cd ..

# Build the wrapper code
echo -e '\x1b[32mCompiling wrapper.c'
musl-gcc -c -O2 -fno-omit-frame-pointer -fexceptions -o build/wrapper.o wrapper.c
# musl-gcc -E -c -O2 -fno-omit-frame-pointer -fexceptions -o build/wrapper.expanded.c wrapper.c

# Create the final library
echo -e '\x1b[32mCreating libunwind-wrapper.a\x1b[0m'
ar rcs libunwind-wrapper.a build/wrapper.o unpacked/*.o

# Build the example code
echo -e '\x1b[32mCompiling example.c'
musl-gcc -fno-omit-frame-pointer -ggdb -static example.c -o build/example -L./ -lunwind-wrapper

# Why not run the example as well?
echo -e '\x1b[32mRunning example\x1b[0m'
./build/example

echo -e '\x1b[32mAll done!\x1b[0m'
