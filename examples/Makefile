
# Copyright (c) 2016-2018 massimo morara

# Distributed under the Mit License
# See accompanying file LICENSE
# See https://github.com/massimomorara/yarg for latest version

CC        = clang++
CPPFLAGS  = -c -g -O0 -Wall -Wextra -ansi -pedantic -Wno-unknown-pragmas \
            -std=c++14 -stdlib=libc++ -I. -I../src
LDFLAGS   = -stdlib=libc++
EXES      = $(shell ls *.cpp | sed 's/.cpp//')
YARGH     = ../src/yarg/yarg.h

all: $(EXES)

clean:
	- rm $(EXES) $(EXES:%=%.o) > /dev/null 2>&1

yargDependent = *.o

$(yargDependent): $(YARGH)

%.o: %.cpp Makefile
	$(CC) -o $@ $(CPPFLAGS) $<

$(EXES):%: %.o Makefile	
	$(CC) -o $@ $(LDFLAGS) $<

