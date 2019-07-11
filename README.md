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
2019-07-11-00:11:23.617096 ____________                 _____      ___    __      ______
2019-07-11-00:11:23.617664 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-00:11:23.617671 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-00:11:23.617677 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-00:11:23.617681 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-00:11:23.617710
2019-07-11-00:11:23.617715 Build type: Release
2019-07-11-00:11:23.617727 total loop count: 1000000
2019-07-11-00:11:23.617735 #####
2019-07-11-00:11:23.617740 ##### FastAtomicReader
2019-07-11-00:11:23.617744 #####
2019-07-11-00:11:23.617750 starting test (10 x 100000)
2019-07-11-00:11:23.620042 done ✅
2019-07-11-00:11:23.620109 time elapsed: 0.002276 s
2019-07-11-00:11:23.620128 rate: 4.39367e+08 ± 7.126e+07/s
2019-07-11-00:11:23.620138 per loop: 2.276 ± 0.36914 ns
2019-07-11-00:11:23.620145 #####
2019-07-11-00:11:23.620150 ##### FastAtomicWriter
2019-07-11-00:11:23.620155 #####
2019-07-11-00:11:23.620167 starting test (10 x 100000)
2019-07-11-00:11:23.623969 done ✅
2019-07-11-00:11:23.624119 time elapsed: 0.003758 s
2019-07-11-00:11:23.624139 rate: 2.66099e+08 ± 3.43167e+07/s
2019-07-11-00:11:23.624151 per loop: 3.758 ± 0.48464 ns
2019-07-11-00:11:23.624191 #####
2019-07-11-00:11:23.624198 ##### std::atomic
2019-07-11-00:11:23.624203 #####
2019-07-11-00:11:23.624214 starting test (10 x 100000)
2019-07-11-00:11:23.631571 done ✅
2019-07-11-00:11:23.631618 time elapsed: 0.007343 s
2019-07-11-00:11:23.631630 rate: 1.36184e+08 ± 1.28569e+07/s
2019-07-11-00:11:23.631637 per loop: 7.343 ± 0.693239 ns
2019-07-11-00:11:23.631642 #####
2019-07-11-00:11:23.631645 ##### mutex
2019-07-11-00:11:23.631649 #####
2019-07-11-00:11:23.631657 starting test (10 x 100000)
2019-07-11-00:11:23.676816 done ✅
2019-07-11-00:11:23.676852 time elapsed: 0.045132 s
2019-07-11-00:11:23.676860 rate: 2.21572e+07 ± 718837/s
2019-07-11-00:11:23.676866 per loop: 45.132 ± 1.4642 ns
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
