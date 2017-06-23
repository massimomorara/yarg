
# Copyright (c) 2016, 2017 massimo morara

# Distributed under the Mit License
# See accompanying file LICENSE
# See https://github.com/massimomorara/yarg for latest version

CPP_GCC   = g++
CPP_CLANG = clang++

COMMON_OPTS = -c -g -O0 -Wall -Wextra -ansi -pedantic -Wno-unknown-pragmas
COMMON_DEPS = Makefile
YARGH       = ../src/yarg/yarg.h
IDIR        = -I. -I../src

CPPFLAGS_11_GCC   = $(COMMON_OPTS) $(IDIR) -std=c++11
CPPFLAGS_11_CLANG = $(CPPFLAGS_11_GCC) -stdlib=libc++
CPPFLAGS_14_GCC   = $(COMMON_OPTS) $(IDIR) -std=c++14
CPPFLAGS_14_CLANG = $(CPPFLAGS_14_GCC) -stdlib=libc++

LDFLAGS_11_GCC   =
LDFLAGS_11_CLANG = -stdlib=libc++
LDFLAGS_14_GCC   =
LDFLAGS_14_CLANG = -stdlib=libc++

TESTS_11_GCC   = $(shell ls test_*-*11*gcc*.cpp | sed 's/-.*/-11-gcc/' )
TESTS_11_CLANG = $(shell ls test_*-*11*clang*.cpp | sed 's/-.*/-11-clang/' )
TESTS_14_GCC   = $(shell ls test_*-*14*gcc*.cpp | sed 's/-.*/-14-gcc/' )
TESTS_14_CLANG = $(shell ls test_*-*14*clang*.cpp | sed 's/-.*/-14-clang/' )

all: $(TESTS_11_GCC) $(TESTS_11_CLANG) $(TESTS_14_GCC) $(TESTS_14_CLANG)

clean:
	- rm $(TESTS_11_GCC) $(TESTS_11_GCC:%=%.o) $(TESTS_11_CLANG) \
	     $(TESTS_11_CLANG:%=%.o) $(TESTS_14_GCC) $(TESTS_14_GCC:%=%.o) \
	     $(TESTS_14_CLANG) $(TESTS_14_CLANG:%=%.o) >& /dev/null

$(TESTS_11_GCC):%: %.o test-base-11-gcc.o $(COMMON_DEPS)	
	$(CPP_GCC) -o $@ $(LDFLAGS_11_GCC) test-base-11-gcc.o $<

$(TESTS_11_CLANG):%: %.o test-base-11-clang.o $(COMMON_DEPS)	
	$(CPP_CLANG) -o $@ $(LDFLAGS_11_CLANG) test-base-11-clang.o $<

$(TESTS_14_GCC):%: %.o test-base-14-gcc.o $(COMMON_DEPS)	
	$(CPP_GCC) -o $@ $(LDFLAGS_14_GCC) test-base-14-gcc.o $<

$(TESTS_14_CLANG):%: %.o test-base-14-clang.o $(COMMON_DEPS)	
	$(CPP_CLANG) -o $@ $(LDFLAGS_14_CLANG) test-base-14-clang.o $<

yargDependent = test_*.o

$(yargDependent): $(YARGH)

%-11-gcc.o: %-*11*gcc*.cpp $(COMMON_DEPS)
	$(CPP_GCC) -o $@ $(CPPFLAGS_11_GCC) $<

%-11-clang.o: %-*11*clang*.cpp $(COMMON_DEPS)
	$(CPP_CLANG) -o $@ $(CPPFLAGS_11_CLANG) $<

%-14-gcc.o: %-*14*gcc*.cpp $(COMMON_DEPS)
	$(CPP_GCC) -o $@ $(CPPFLAGS_14_GCC) $<

%-14-clang.o: %-*14*clang*.cpp $(COMMON_DEPS)
	$(CPP_CLANG) -o $@ $(CPPFLAGS_14_CLANG) $<

