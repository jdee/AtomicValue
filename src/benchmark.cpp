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

        LOG("total loop count: " << maxCount);

        auto const numPerLoop(maxCount/10);

        TestLoop<FastAtomicReader>::loop(10, numPerLoop);
        TestLoop<FastAtomicWriter>::loop(10, numPerLoop);
        TestLoop<MySTLAtomic>::loop(10, numPerLoop);
        TestLoop<MutexWrapper>::loop(10, numPerLoop);

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
