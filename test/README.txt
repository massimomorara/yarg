
1) Catch

To check Yarg you need Catch (see https://github.com/philsquared/Catch)

A simple solution could be download, and place in this directory, the single
include version (the file catch.hpp from directory

https://raw.githubusercontent.com/philsquared/Catch/master/single_include

at the moment) or add the directory, where catch.hpp, in IDIR (see Makefile).


2) Choose compiler(s) and C++ version(s)

In this directory you find a Makefile and two cpp files
(test-base-11,14,gcc,clang.cpp and test_yarg-11,14,gcc,clang.cpp).

Without changes, the "make" command try to create four executables:
- test_yarg-11-clang, compiled with clang++ using C++11
- test_yarg-11-gcc, compiled with g++ using C++11
- test_yarg-14-clang, compiled with clang++ using C++14
- test_yarg-14-gcc, compiled with g++ using C++14

If you don't have one of the two compiler (or you don't want to use it) you
can simply rename the cpp files removing the part corresponding to the
compiler you want to exclude. 
By example, if you want create only the two clang++ executables, you can
- rename test-base-11,14,gcc,clang.cpp in test-base-11,14,clang.cpp
- rename test_yarg-11,14,gcc,clang.cpp in test_yarg-11,14,clang.cpp

Same solution if you want avoid to produce the executables of one version of
C++ language, you have to remove the corresponding part from cpp names.
By example, if you want create only the two C++14 executables, you can
- rename test-base-11,14,gcc,clang.cpp in test-base-14,gcc,clang.cpp
- rename test_yarg-11,14,gcc,clang.cpp in test_yarg-14,gcc,clang.cpp

You can mix the two alteration types.
By example, if you want create only the g++ C++11 executable, you can
- rename test-base-11,14,gcc,clang.cpp in test-base-11,gcc.cpp
- rename test_yarg-11,14,gcc,clang.cpp in test_yarg-11,gcc.cpp
