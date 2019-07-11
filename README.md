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
2019-07-11-01:26:59.257333 ____________                 _____      ___    __      ______
2019-07-11-01:26:59.257900 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-01:26:59.257906 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-01:26:59.257911 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-01:26:59.257915 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-01:26:59.257939
2019-07-11-01:26:59.257947 Build type: Release
2019-07-11-01:26:59.257957 total loop count: 1000000
2019-07-11-01:26:59.257963 #####
2019-07-11-01:26:59.257967 ##### FastAtomicReader
2019-07-11-01:26:59.257970 #####
2019-07-11-01:26:59.257975 starting test (100 x 10000)
2019-07-11-01:26:59.261200 done ✅
2019-07-11-01:26:59.261240 time elapsed: 0.001771 s
2019-07-11-01:26:59.261251 rate: 5.64653e+08 ± 1.24625e+07 Hz (σ = 1.24625e+08 Hz)
2019-07-11-01:26:59.261266 per loop: 1.771 ± 0.039088 ns (σ = 0.39088 ns)
2019-07-11-01:26:59.261272 #####
2019-07-11-01:26:59.261277 ##### FastAtomicWriter
2019-07-11-01:26:59.261280 #####
2019-07-11-01:26:59.261286 starting test (100 x 10000)
2019-07-11-01:26:59.265440 done ✅
2019-07-11-01:26:59.265486 time elapsed: 0.00251 s
2019-07-11-01:26:59.265496 rate: 3.98406e+08 ± 5.55146e+06 Hz (σ = 5.55146e+07 Hz)
2019-07-11-01:26:59.265505 per loop: 2.51 ± 0.0349747 ns (σ = 0.349747 ns)
2019-07-11-01:26:59.265510 #####
2019-07-11-01:26:59.265519 ##### std::atomic
2019-07-11-01:26:59.265523 #####
2019-07-11-01:26:59.265531 starting test (100 x 10000)
2019-07-11-01:26:59.272030 done ✅
2019-07-11-01:26:59.272074 time elapsed: 0.006464 s
2019-07-11-01:26:59.272084 rate: 1.54703e+08 ± 1.78856e+06 Hz (σ = 1.78856e+07 Hz)
2019-07-11-01:26:59.272101 per loop: 6.464 ± 0.0747321 ns (σ = 0.747321 ns)
2019-07-11-01:26:59.272107 #####
2019-07-11-01:26:59.272112 ##### mutex
2019-07-11-01:26:59.272116 #####
2019-07-11-01:26:59.272120 starting test (100 x 10000)
2019-07-11-01:26:59.312931 done ✅
2019-07-11-01:26:59.312980 time elapsed: 0.040662 s
2019-07-11-01:26:59.312990 rate: 2.4593e+07 ± 160099 Hz (σ = 1.60099e+06 Hz)
2019-07-11-01:26:59.312998 per loop: 40.662 ± 0.264708 ns (σ = 2.64708 ns)
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
