simple-strconv
======
simple-strconv is a C/C++ library that provides string-to-integer and
integer-to-string conversion functions for native int types.

The semantics are slightly different than those of the C library's `strtoX`
and `sscanf` functions. In particular, `errno` is not used, partial conversions
are not allowed, and the functions are stricter about overflow. For details, see
the documentation in the header files.

Compiling
---------
To build simple-strconv, you need the following on your system.

*   [CMake](http://www.cmake.org)
*   A compiler that supports C++11.

Invoke `cmake [...] <srcdir>` to generate build files for your system.
