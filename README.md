```
____________                 _____      ___    __      ______
___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
__  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
_  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
/_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
```

# AtomicValue

FastAtomicReader and FastAtomicWriter templates for C++.

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
2019-06-20-23:03:24.908023 ____________                 _____      ___    __      ______
2019-06-20-23:03:24.908615 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-06-20-23:03:24.908624 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-06-20-23:03:24.908631 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-06-20-23:03:24.908638 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-06-20-23:03:24.908676
2019-06-20-23:03:24.908684 Build type: Release
2019-06-20-23:03:24.908698 loop count: 1000000
2019-06-20-23:03:24.908708 #####
2019-06-20-23:03:24.908713 ##### FastAtomicReader
2019-06-20-23:03:24.908719 #####
2019-06-20-23:03:24.908726 starting test
2019-06-20-23:03:24.910677 done ✅
2019-06-20-23:03:24.910701 final counter value: 1000000
2019-06-20-23:03:24.910718 time elapsed: 0.001944 s
2019-06-20-23:03:24.910727 rate: 5.14403e+08/s
2019-06-20-23:03:24.910737 per loop: 1.944 ns
2019-06-20-23:03:24.910785 #####
2019-06-20-23:03:24.910821 ##### FastAtomicWriter
2019-06-20-23:03:24.910829 #####
2019-06-20-23:03:24.910835 starting test
2019-06-20-23:03:24.914031 done ✅
2019-06-20-23:03:24.914062 final counter value: 1000000
2019-06-20-23:03:24.914075 time elapsed: 0.00318 s
2019-06-20-23:03:24.914085 rate: 3.14465e+08/s
2019-06-20-23:03:24.914093 per loop: 3.18 ns
2019-06-20-23:03:24.914190 #####
2019-06-20-23:03:24.914202 ##### mutex
2019-06-20-23:03:24.914209 #####
2019-06-20-23:03:24.914214 starting test
2019-06-20-23:03:24.963720 done ✅
2019-06-20-23:03:24.963776 final counter value: 1000000
2019-06-20-23:03:24.963790 time elapsed: 0.049494 s
2019-06-20-23:03:24.963799 rate: 2.02045e+07/s
2019-06-20-23:03:24.963806 per loop: 49.494 ns
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
# compiles
cout << "x = " << x.set(1) << endl;
```

but:

```cpp
FastAtomicReader<int> volatile x;
# does not compile
cout << "x = " << x.set(1) << endl;
```

Instead:

```cpp
FastAtomicReader<int> volatile x;
# compiles
x.set(1);
cout << "x = " << x << endl;
```

Operators/functions that return void on volatile objects:

- `set(T)`
- `operator+=(int)`
- `operator++()`
- `operator=(T)`
- `operator=(const FastAtomicReader/Writer&)`
