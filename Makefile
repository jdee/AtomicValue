CPPFLAGS=-Iinclude # -DVOLATILE_COUNTERS
CXXFLAGS=-std=c++11

# Release
CXXFLAGS+=-O3 -Wall -Werror

# Debug
# CPPFLAGS+=-DDEBUG
# CXXFLAGS+=-O0 -g

LDLIBS=-lpthread

OBJECTS=src/benchmark.o src/log.o src/timeutil.o

all: benchmark

benchmark: $(OBJECTS)
	$(LINK.cc) $(OBJECTS) $(LOADLIBES) $(LDLIBS) -o $@

check: all
	./benchmark 10000

clean:
	$(RM) benchmark src/*.o

src/benchmark.o: Makefile \
	include/AtomicValue/AtomicValue.h \
	include/Bare.h \
	include/log.h \
	include/timeutil.h \
	include/Metadata.h \
	include/MutexWrapper.h \
	include/MySTLAtomic.h \
	include/TestLoop.h \
	include/static_assert.h \
	include/Accumulator.h

src/log.o: Makefile \
	include/log.h \
	include/timeutil.h

src/timeutil.o: Makefile \
	include/timeutil.h
