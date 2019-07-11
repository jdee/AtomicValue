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
2019-07-11-02:16:37.271437 ____________                 _____      ___    __      ______
2019-07-11-02:16:37.271931 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-07-11-02:16:37.271938 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-07-11-02:16:37.271942 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-07-11-02:16:37.271946 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-07-11-02:16:37.271968
2019-07-11-02:16:37.271973 Build type: Release
2019-07-11-02:16:37.271983 total loop count: 1000000
2019-07-11-02:16:37.271989 #####
2019-07-11-02:16:37.271993 ##### unsynchronized
2019-07-11-02:16:37.271996 #####
2019-07-11-02:16:37.272001 starting test (100 x 10000)
2019-07-11-02:16:37.273817 done ✅
2019-07-11-02:16:37.273839 time elapsed: 0.001801 s
2019-07-11-02:16:37.273847 rate: 5.55247e+08 ± 3.93598e+06 Hz (σ = 3.95581e+07 Hz)
2019-07-11-02:16:37.273853 per loop: 1.801 ± 0.0127668 ns (σ = 0.128311 ns)
2019-07-11-02:16:37.273857 #####
2019-07-11-02:16:37.273862 ##### FastAtomicReader
2019-07-11-02:16:37.273866 #####
2019-07-11-02:16:37.273871 starting test (100 x 10000)
2019-07-11-02:16:37.277011 done ✅
2019-07-11-02:16:37.277028 time elapsed: 0.001671 s
2019-07-11-02:16:37.277035 rate: 5.98444e+08 ± 1.3864e+07 Hz (σ = 1.39338e+08 Hz)
2019-07-11-02:16:37.277041 per loop: 1.671 ± 0.0387116 ns (σ = 0.389066 ns)
2019-07-11-02:16:37.277049 #####
2019-07-11-02:16:37.277053 ##### FastAtomicWriter
2019-07-11-02:16:37.277056 #####
2019-07-11-02:16:37.277061 starting test (100 x 10000)
2019-07-11-02:16:37.280734 done ✅
2019-07-11-02:16:37.280775 time elapsed: 0.002209 s
2019-07-11-02:16:37.280782 rate: 4.52694e+08 ± 4.76301e+06 Hz (σ = 4.78701e+07 Hz)
2019-07-11-02:16:37.280789 per loop: 2.209 ± 0.023242 ns (σ = 0.233591 ns)
2019-07-11-02:16:37.280793 #####
2019-07-11-02:16:37.280796 ##### std::atomic
2019-07-11-02:16:37.280802 #####
2019-07-11-02:16:37.280806 starting test (100 x 10000)
2019-07-11-02:16:37.286740 done ✅
2019-07-11-02:16:37.286783 time elapsed: 0.005922 s
2019-07-11-02:16:37.286791 rate: 1.68862e+08 ± 1.02142e+06 Hz (σ = 1.02656e+07 Hz)
2019-07-11-02:16:37.286798 per loop: 5.922 ± 0.0358212 ns (σ = 0.360017 ns)
2019-07-11-02:16:37.286806 #####
2019-07-11-02:16:37.286810 ##### mutex
2019-07-11-02:16:37.286814 #####
2019-07-11-02:16:37.286819 starting test (100 x 10000)
2019-07-11-02:16:37.326793 done ✅
2019-07-11-02:16:37.326828 time elapsed: 0.039875 s
2019-07-11-02:16:37.326836 rate: 2.50784e+07 ± 73610.4 Hz (σ = 739812 Hz)
2019-07-11-02:16:37.326843 per loop: 39.875 ± 0.117042 ns (σ = 1.17631 ns)
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
