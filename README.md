# A small wrapper around libunwind

This project is a small hack intended to suit my use case which is:
- support backtraces in musl based projects
- don't introduce a direct dependcency on libunwind
- compile under x86_64

`build.sh` script contains a snippet of how you can compile libunwind with musl.

`wrapper.c` contains some functions which can be used to access libunwind's
functionality without knowing any definitions from libunwind itself (struct
pointers could be replaced with `void *`).

`example.c` contains a small example

`libunwind-wrapper.a` is a musl based libunwind.a + libunwind-x86_64.a + wrapper.o
