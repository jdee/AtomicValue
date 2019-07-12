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
2019-07-11-23:14:51.774524 ____________                 _____      ___    __      ______
2019-07-11-23:14:51.775160 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-23:14:51.775166 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-23:14:51.775171 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-23:14:51.775175 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-23:14:51.775196
2019-07-11-23:14:51.775200 Build type: Release
2019-07-11-23:14:51.775209 loop count: 10000
2019-07-11-23:14:51.775215 #####
2019-07-11-23:14:51.775219 ##### unsynchronized
2019-07-11-23:14:51.775222 #####
2019-07-11-23:14:51.775227 starting test (100 x 10000)
2019-07-11-23:14:51.777242 done ✅
2019-07-11-23:14:51.777275 time elapsed: 2.004 ms
2019-07-11-23:14:51.777285 rate: 499.002 ± 22.4951 MHz (σ = 224.951 MHz)
2019-07-11-23:14:51.777291 per loop: 2.004 ± 0.0903408 ns (σ = 0.903408 ns)
2019-07-11-23:14:51.777297 #####
2019-07-11-23:14:51.777301 ##### FastAtomicReader
2019-07-11-23:14:51.777304 #####
2019-07-11-23:14:51.777308 starting test (100 x 10000)
2019-07-11-23:14:51.780451 done ✅
2019-07-11-23:14:51.780486 time elapsed: 1.665 ms
2019-07-11-23:14:51.780494 rate: 600.601 ± 19.6667 MHz (σ = 196.667 MHz)
2019-07-11-23:14:51.780521 per loop: 1.665 ± 0.0545204 ns (σ = 0.545204 ns)
2019-07-11-23:14:51.780527 #####
2019-07-11-23:14:51.780530 ##### FastAtomicWriter
2019-07-11-23:14:51.780534 #####
2019-07-11-23:14:51.780538 starting test (100 x 10000)
2019-07-11-23:14:51.784263 done ✅
2019-07-11-23:14:51.784280 time elapsed: 2.247 ms
2019-07-11-23:14:51.784288 rate: 445.038 ± 9.80625 MHz (σ = 98.0625 MHz)
2019-07-11-23:14:51.784296 per loop: 2.247 ± 0.0495119 ns (σ = 0.495119 ns)
2019-07-11-23:14:51.784307 #####
2019-07-11-23:14:51.784311 ##### std::atomic
2019-07-11-23:14:51.784315 #####
2019-07-11-23:14:51.784320 starting test (100 x 10000)
2019-07-11-23:14:51.790396 done ✅
2019-07-11-23:14:51.790428 time elapsed: 6.066 ms
2019-07-11-23:14:51.790435 rate: 164.853 ± 3.86797 MHz (σ = 38.6797 MHz)
2019-07-11-23:14:51.790441 per loop: 6.066 ± 0.142327 ns (σ = 1.42327 ns)
2019-07-11-23:14:51.790447 #####
2019-07-11-23:14:51.790451 ##### mutex
2019-07-11-23:14:51.790454 #####
2019-07-11-23:14:51.790458 starting test (100 x 10000)
2019-07-11-23:14:51.829111 done ✅
2019-07-11-23:14:51.829147 time elapsed: 38.563 ms
2019-07-11-23:14:51.829158 rate: 25.9316 ± 0.253256 MHz (σ = 2.53256 MHz)
2019-07-11-23:14:51.829166 per loop: 38.563 ± 0.376619 ns (σ = 3.76619 ns)
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
