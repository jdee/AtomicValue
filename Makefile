CPPFLAGS=-Iinclude
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
	./benchmark 1000000

clean:
	$(RM) benchmark src/*.o

src/benchmark.o: include/AtomicValue/AtomicValue.h \
	include/log.h \
	include/timeutil.h \
	include/Metadata.h \
	include/MutexWrapper.h \
	include/MySTLAtomic.h

src/log.o: include/log.h \
	include/timeutil.h

src/timeutil.o: include/timeutil.h
