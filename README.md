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
2019-06-20-21:14:10.928685 ____________                 _____      ___    __      ______
2019-06-20-21:14:10.929249 ___    |_  /_____________ ______(_)_______ |  / /_____ ___  /___  ______
2019-06-20-21:14:10.929256 __  /| |  __/  __ \_  __ `__ \_  /_  ___/_ | / /_  __ `/_  /_  / / /  _ \
2019-06-20-21:14:10.929261 _  ___ / /_ / /_/ /  / / / / /  / / /__ __ |/ / / /_/ /_  / / /_/ //  __/
2019-06-20-21:14:10.929265 /_/  |_\__/ \____//_/ /_/ /_//_/  \___/ _____/  \__,_/ /_/  \__,_/ \___/
2019-06-20-21:14:10.929293
2019-06-20-21:14:10.929299 Build type: Release
2019-06-20-21:14:10.929309 loop count: 1000000
2019-06-20-21:14:10.929317 #####
2019-06-20-21:14:10.929320 ##### AtomicValue
2019-06-20-21:14:10.929324 #####
2019-06-20-21:14:10.929329 starting test
2019-06-20-21:14:10.931318 done ✅
2019-06-20-21:14:10.931356 final counter value: 1000000
2019-06-20-21:14:10.931371 time elapsed: 0.001983 s
2019-06-20-21:14:10.931377 rate: 5.04286e+08/s
2019-06-20-21:14:10.931382 per loop: 1.983 ns
2019-06-20-21:14:10.931508 #####
2019-06-20-21:14:10.931542 ##### mutex
2019-06-20-21:14:10.931549 #####
2019-06-20-21:14:10.931556 starting test
2019-06-20-21:14:10.977481 done ✅
2019-06-20-21:14:10.977557 final counter value: 1000000
2019-06-20-21:14:10.977572 time elapsed: 0.045907 s
2019-06-20-21:14:10.977580 rate: 2.17832e+07/s
2019-06-20-21:14:10.977600 per loop: 45.907 ns
```

## Usage

```cpp
#include <iostream>
#include <AtomicValue/AtomicValue.h>

AtomicValue::FastAtomicReader<unsigned int> counter(0);

++ counter;

cout << "counter = " << counter << endl;

```
