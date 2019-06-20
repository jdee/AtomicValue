INCLUDES=-Iinclude

# Release
CPPFLAGS=$(INCLUDES) -O3 -s

# Debug
# CPPFLAGS=$(INCLUDES) -O0 -g -DDEBUG

all: test

check: test
	./test

clean:
	rm -f test

test: include/AtomicValue/AtomicValue.h
