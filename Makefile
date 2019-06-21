CPPFLAGS=-Iinclude

CXXFLAGS=-std=c++11 -Wall -Werror

# Release
CXXFLAGS+=-O3

# Debug
# CXXFLAGS+=-O0 -g
# CPPFLAGS+=-DDEBUG

all: test

OBJECTS=src/test.o src/timeutil.o

test: $(OBJECTS)
	$(LINK.cc) $(LOADLIBES) $(LDLIBS) $(OBJECTS) -o $@

check: test
	./test 1000000

clean:
	$(RM) test *.o

src/test.o: include/AtomicValue/AtomicValue.h \
	include/timeutil.h \
	include/MutexWrapper.h

src/timeutil.o: include/timeutil.h
