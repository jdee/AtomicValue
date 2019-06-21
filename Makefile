CPPFLAGS=-Iinclude

CXXFLAGS=-std=c++11 -Wall -Werror

# Release
CXXFLAGS+=-O3

# Debug
# CXXFLAGS+=-O0 -g
# CPPFLAGS+=-DDEBUG

all: benchmark

OBJECTS=src/benchmark.o src/timeutil.o

benchmark: $(OBJECTS)
	$(LINK.cc) $(LOADLIBES) $(LDLIBS) $(OBJECTS) -o $@

check: benchmark
	./benchmark 1000000

clean:
	$(RM) benchmark *.o

src/benchmark.o: include/AtomicValue/AtomicValue.h \
	include/timeutil.h \
	include/MutexWrapper.h

src/timeutil.o: include/timeutil.h
