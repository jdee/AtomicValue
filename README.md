```
____________                 _____      ___    __      ______
___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
__  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
_  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
/_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
```

# AtomicValue

FastAtomicReader and FastAtomicWriter templates for C++. Currently limited to
POSIX. Tested on iOS, macOS and Ubuntu. Limited features. Highly unsafe.

FastAtomicReader and FastAtomicWriter are lightweight, asymmetric
synchronization primitives similar to std::atomic. In each case, either read
or write operations are undelayable. Locking is accomplished using only an
integer counter. The FastAtomicReader template provides a shared read lock, but
not an exclusive write lock. FastAtomicWriter is the reverse. It provides a
shared write lock with no read lock (and hence is best used when only one
thread will ever perform write operations, and reads are few).

While theoretically unsafe, these templates have been in use in an iOS app on
the App Store for five years without event. They are prone to a peculiar kind
of deadlock, but that was addressed before the app was launched. Since then,
they have performed admirably in a highly latency-sensitve app where they are
used extensively.

They consistenly beat std::atomic in benchmarking by a significant factor, but
at the cost of safety. Use with care!

## Benchmark

```bash
make check
```

```
[jdee@Jimmy-Dees-MacBookPro AtomicValue (master)]$ make check
c++ -std=c++11 -O3 -Iinclude -Wall -Werror  -c -o src/benchmark.o src/benchmark.cpp
c++ -std=c++11 -O3 -Iinclude -Wall -Werror  -c -o src/timeutil.o src/timeutil.cpp
c++ -std=c++11 -O3 -Iinclude -Wall -Werror   src/benchmark.o src/timeutil.o  -lpthread -o benchmark
./benchmark 1000000
2019-06-21-12:04:10.352465 ____________                 _____      ___    __      ______
2019-06-21-12:04:10.352978 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-06-21-12:04:10.352986 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-06-21-12:04:10.352992 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-06-21-12:04:10.352997 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-06-21-12:04:10.353036
2019-06-21-12:04:10.353051 Build type: Release
2019-06-21-12:04:10.353064 loop count: 1000000
2019-06-21-12:04:10.353072 #####
2019-06-21-12:04:10.353075 ##### FastAtomicReader
2019-06-21-12:04:10.353079 #####
2019-06-21-12:04:10.353082 starting test
2019-06-21-12:04:10.354784 done ✅
2019-06-21-12:04:10.354802 final counter value: 1000000
2019-06-21-12:04:10.354815 time elapsed: 0.001698 s
2019-06-21-12:04:10.354824 rate: 5.88928e+08/s
2019-06-21-12:04:10.354829 per loop: 1.698 ns
2019-06-21-12:04:10.354855 #####
2019-06-21-12:04:10.354859 ##### FastAtomicWriter
2019-06-21-12:04:10.354877 #####
2019-06-21-12:04:10.354905 starting test
2019-06-21-12:04:10.357091 done ✅
2019-06-21-12:04:10.357121 final counter value: 1000000
2019-06-21-12:04:10.357128 time elapsed: 0.002178 s
2019-06-21-12:04:10.357132 rate: 4.59137e+08/s
2019-06-21-12:04:10.357140 per loop: 2.178 ns
2019-06-21-12:04:10.357210 #####
2019-06-21-12:04:10.357285 ##### std::atomic
2019-06-21-12:04:10.357293 #####
2019-06-21-12:04:10.357318 starting test
2019-06-21-12:04:10.363453 done ✅
2019-06-21-12:04:10.363494 final counter value: 1000000
2019-06-21-12:04:10.363504 time elapsed: 0.006107 s
2019-06-21-12:04:10.363510 rate: 1.63747e+08/s
2019-06-21-12:04:10.363516 per loop: 6.107 ns
2019-06-21-12:04:10.363587 #####
2019-06-21-12:04:10.363595 ##### mutex
2019-06-21-12:04:10.363600 #####
2019-06-21-12:04:10.363605 starting test
2019-06-21-12:04:10.401690 done ✅
2019-06-21-12:04:10.401716 final counter value: 1000000
2019-06-21-12:04:10.401728 time elapsed: 0.038076 s
2019-06-21-12:04:10.401733 rate: 2.62633e+07/s
2019-06-21-12:04:10.401737 per loop: 38.076 ns
```

## Usage

```cpp
#include <iostream>
#include <AtomicValue/AtomicValue.h>

AtomicValue::FastAtomicReader<unsigned int> counter(0);

++ counter;

cout << "counter = " << counter << endl;

```

## Use of volatile

A FastAtomicReader or FastAtomicWriter may be declared `volatile`. However, note
that certain operators and member functions that would ordinarily return
references to `*this` would have to return references to volatiles. This
generates warnings from G++ in calling code in situations like this:

```cpp
FastAtomicReader<int> volatile x;
x = 1;
```

The return type of the assignment would be `FastAtomicReader<int> volatile&`.
The return value is unread, which generates a warning at the second line. The
warning can't be silenced in the AtomicValue source code. As a result, the
volatile versions of these functions and operators do not return a value, e.g.

```cpp
FastAtomicReader& set(T data);
void set(T data) volatile;
```

So:

```cpp
FastAtomicReader<int> x;
// compiles
cout << "x = " << x.set(1) << endl;
```

but:

```cpp
FastAtomicReader<int> volatile x;
// does not compile
cout << "x = " << x.set(1) << endl;
```

Instead:

```cpp
FastAtomicReader<int> volatile x;
// compiles
x.set(1);
cout << "x = " << x << endl;
```

Operators/functions that return void on volatile objects:

- `set(T)`
- `operator+=(int)`
- `operator++()`
- `operator=(T)`
- `operator=(const FastAtomicReader/Writer&)`
