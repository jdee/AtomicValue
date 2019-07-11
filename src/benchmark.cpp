#include <algorithm>
#include <cstdlib>
#include <exception>

#include <AtomicValue/AtomicValue.h>

#include "MutexWrapper.h"
#include "MySTLAtomic.h"
#include "TestLoop.h"
#include "log.h"
#include "timeutil.h"

using namespace AtomicValue;
using namespace std;

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif // DEBUG

unsigned long long
getMaxCount(int argc, char** argv)
{
    if (argc < 2) return 10000000000;
    return max(0LL, atoll(argv[1]));
}

int
main(int argc, char** argv)
{
    try
    {
        logBanner();
        LOG("");
        LOG("Build type: " << BUILD_TYPE);
#ifdef VOLATILE_COUNTERS
        LOG("Volatile counters");
#endif // VOLATILE_COUNTER

        const unsigned long long maxCount(getMaxCount(argc, argv));

        LOG("loop count: " << maxCount);

        TestLoop<FastAtomicReader>::run(maxCount);
        TestLoop<FastAtomicWriter>::run(maxCount);
        TestLoop<MySTLAtomic>::run(maxCount);
        TestLoop<MutexWrapper>::run(maxCount);

        return 0;
    }
    catch (exception& e)
    {
        LOG("Exception in main: " << e.what());
        return -1;
    }
    catch (...)
    {
        LOG("Unexpected exception in main. Aborting.");
        abort();
    }
}
