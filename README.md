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
2019-07-11-01:48:37.049360 ____________                 _____      ___    __      ______
2019-07-11-01:48:37.049950 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-01:48:37.049957 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-01:48:37.049961 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-01:48:37.049965 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-01:48:37.049987
2019-07-11-01:48:37.049991 Build type: Release
2019-07-11-01:48:37.050000 total loop count: 1000000
2019-07-11-01:48:37.050007 #####
2019-07-11-01:48:37.050011 ##### unsynchronized
2019-07-11-01:48:37.050014 #####
2019-07-11-01:48:37.050019 starting test (100 x 10000)
2019-07-11-01:48:37.050030 done ✅
2019-07-11-01:48:37.050038 time elapsed: 2e-06 s
2019-07-11-01:48:37.050044 rate: 5e+11 ± 3.5e+11 Hz (σ = 3.51763e+12 Hz)
2019-07-11-01:48:37.050050 per loop: 0.002 ± 0.0014 ns (σ = 0.0140705 ns)
2019-07-11-01:48:37.050054 #####
2019-07-11-01:48:37.050059 ##### FastAtomicReader
2019-07-11-01:48:37.050062 #####
2019-07-11-01:48:37.050066 starting test (100 x 10000)
2019-07-11-01:48:37.053259 done ✅
2019-07-11-01:48:37.053289 time elapsed: 0.001776 s
2019-07-11-01:48:37.053296 rate: 5.63063e+08 ± 2.35171e+07 Hz (σ = 2.36356e+08 Hz)
2019-07-11-01:48:37.053302 per loop: 1.776 ± 0.0741771 ns (σ = 0.745508 ns)
2019-07-11-01:48:37.053310 #####
2019-07-11-01:48:37.053313 ##### FastAtomicWriter
2019-07-11-01:48:37.053317 #####
2019-07-11-01:48:37.053321 starting test (100 x 10000)
2019-07-11-01:48:37.056890 done ✅
2019-07-11-01:48:37.056908 time elapsed: 0.002191 s
2019-07-11-01:48:37.056915 rate: 4.56413e+08 ± 5.75859e+06 Hz (σ = 5.7876e+07 Hz)
2019-07-11-01:48:37.056921 per loop: 2.191 ± 0.027644 ns (σ = 0.277833 ns)
2019-07-11-01:48:37.056925 #####
2019-07-11-01:48:37.056929 ##### std::atomic
2019-07-11-01:48:37.056934 #####
2019-07-11-01:48:37.056938 starting test (100 x 10000)
2019-07-11-01:48:37.063467 done ✅
2019-07-11-01:48:37.063503 time elapsed: 0.00652 s
2019-07-11-01:48:37.063510 rate: 1.53374e+08 ± 4.32234e+06 Hz (σ = 4.34411e+07 Hz)
2019-07-11-01:48:37.063516 per loop: 6.52 ± 0.183744 ns (σ = 1.8467 ns)
2019-07-11-01:48:37.063523 #####
2019-07-11-01:48:37.063526 ##### mutex
2019-07-11-01:48:37.063530 #####
2019-07-11-01:48:37.063534 starting test (100 x 10000)
2019-07-11-01:48:37.105425 done ✅
2019-07-11-01:48:37.105464 time elapsed: 0.041788 s
2019-07-11-01:48:37.105472 rate: 2.39303e+07 ± 262314 Hz (σ = 2.63635e+06 Hz)
2019-07-11-01:48:37.105478 per loop: 41.788 ± 0.458062 ns (σ = 4.60369 ns)
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
