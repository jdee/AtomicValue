CPPFLAGS=-Iinclude

CXXFLAGS=-std=c++11

# Release
CXXFLAGS+=-O3
CPPFLAGS+=-Wall -Werror

# Debug
# CXXFLAGS+=-O0 -g
# CPPFLAGS+=-DDEBUG

LDLIBS=-lpthread

all: benchmark

OBJECTS=src/benchmark.o src/timeutil.o

benchmark: $(OBJECTS)
	$(LINK.cc) $(OBJECTS) $(LOADLIBES) $(LDLIBS) -o $@

check: all
	./benchmark 1000000

clean:
	$(RM) benchmark src/*.o

src/benchmark.o: include/AtomicValue/AtomicValue.h \
	include/timeutil.h \
	include/MutexWrapper.h

src/timeutil.o: include/timeutil.h
