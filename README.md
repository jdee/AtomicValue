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

While theoretically unsafe, these templates have been in use in [an iOS app on
the App Store](https://itunes.apple.com/us/app/cpyn/id929721548?mt=8) for five
years without event. They are prone to a peculiar kind
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
c++ -std=c++11 -O3 -Wall -Werror -Iinclude   -c -o src/benchmark.o src/benchmark.cpp
c++ -std=c++11 -O3 -Wall -Werror -Iinclude   -c -o src/log.o src/log.cpp
c++ -std=c++11 -O3 -Wall -Werror -Iinclude   -c -o src/timeutil.o src/timeutil.cpp
c++ -std=c++11 -O3 -Wall -Werror -Iinclude    src/benchmark.o src/log.o src/timeutil.o  -lpthread -o benchmark
./benchmark 1000000
2019-07-11-01:31:37.922527 ____________                 _____      ___    __      ______
2019-07-11-01:31:37.923081 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-01:31:37.923088 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-01:31:37.923094 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-01:31:37.923098 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-01:31:37.923129
2019-07-11-01:31:37.923138 Build type: Release
2019-07-11-01:31:37.923149 total loop count: 1000000
2019-07-11-01:31:37.923154 #####
2019-07-11-01:31:37.923160 ##### FastAtomicReader
2019-07-11-01:31:37.923164 #####
2019-07-11-01:31:37.923170 starting test (100 x 10000)
2019-07-11-01:31:37.926862 done ✅
2019-07-11-01:31:37.926898 time elapsed: 0.002113 s
2019-07-11-01:31:37.926907 rate: 4.73261e+08 ± 1.16021e+07 Hz (σ = 1.16605e+08 Hz)
2019-07-11-01:31:37.926920 per loop: 2.113 ± 0.0518007 ns (σ = 0.520616 ns)
2019-07-11-01:31:37.926925 #####
2019-07-11-01:31:37.926929 ##### FastAtomicWriter
2019-07-11-01:31:37.926933 #####
2019-07-11-01:31:37.926938 starting test (100 x 10000)
2019-07-11-01:31:37.932136 done ✅
2019-07-11-01:31:37.932167 time elapsed: 0.003124 s
2019-07-11-01:31:37.932176 rate: 3.20102e+08 ± 6.7193e+06 Hz (σ = 6.75315e+07 Hz)
2019-07-11-01:31:37.932183 per loop: 3.124 ± 0.0655762 ns (σ = 0.659066 ns)
2019-07-11-01:31:37.932189 #####
2019-07-11-01:31:37.932196 ##### std::atomic
2019-07-11-01:31:37.932200 #####
2019-07-11-01:31:37.932205 starting test (100 x 10000)
2019-07-11-01:31:37.940121 done ✅
2019-07-11-01:31:37.940180 time elapsed: 0.007879 s
2019-07-11-01:31:37.940194 rate: 1.2692e+08 ± 1.52655e+06 Hz (σ = 1.53424e+07 Hz)
2019-07-11-01:31:37.940228 per loop: 7.879 ± 0.094766 ns (σ = 0.952434 ns)
2019-07-11-01:31:37.940237 #####
2019-07-11-01:31:37.940243 ##### mutex
2019-07-11-01:31:37.940249 #####
2019-07-11-01:31:37.940257 starting test (100 x 10000)
2019-07-11-01:31:37.987294 done ✅
2019-07-11-01:31:37.987334 time elapsed: 0.046886 s
2019-07-11-01:31:37.987342 rate: 2.13283e+07 ± 82893.8 Hz (σ = 833114 Hz)
2019-07-11-01:31:37.987349 per loop: 46.886 ± 0.182225 ns (σ = 1.83143 ns)
```

## Usage

```cpp
#include <iostream>
#include <AtomicValue/AtomicValue.h>

using namespace std;

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
