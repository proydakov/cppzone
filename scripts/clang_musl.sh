clang -O3 -static -nostdinc -nodefaultlibs --sysroot /usr/local/musl -I/usr/local/musl/include hello.c -L/usr/local/musl/lib -lc -o hello_musl_clang -v
