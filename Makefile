CPPFLAGS=-Iinclude

# Release
CXXFLAGS=-O3

# Debug
# CXXFLAGS=-O0 -g
# CPPFLAGS+=-DDEBUG

all: test

test: test.o
	$(LINK.cc) $(LOADLIBES) $(LDLIBS) $< -o $@

check: test
	./test

clean:
	$(RM) test *.o

test.o: include/AtomicValue/AtomicValue.h
