CPPFLAGS=-Iinclude

CXXFLAGS=-std=c++11 -Wall -Werror

# Release
CXXFLAGS+=-O3

# Debug
# CXXFLAGS+=-O0 -g
# CPPFLAGS+=-DDEBUG

all: test

test: test.o
	$(LINK.cc) $(LOADLIBES) $(LDLIBS) $< -o $@

check: test
	./test 1000000

clean:
	$(RM) test *.o

test.o: include/AtomicValue/AtomicValue.h mutexwrapper.h
