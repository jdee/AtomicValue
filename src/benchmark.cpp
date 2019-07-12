#include <algorithm>
#include <cstdlib>
#include <exception>

#include <AtomicValue/AtomicValue.h>

#include "Bare.h"
#include "MutexWrapper.h"
#include "MySTLAtomic.h"
#include "TestLoop.h"
#include "log.h"

using namespace AtomicValue;
using namespace std;

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif // DEBUG

static unsigned long long
getLoopCount(int argc, char** argv)
{
    if (argc < 2) return 10000000;
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

        auto const loopCount(getLoopCount(argc, argv));

        LOG("loop count: " << loopCount);

        TestLoop<Bare>::loop(loopCount);
        TestLoop<FastAtomicReader>::loop(loopCount);
        TestLoop<FastAtomicWriter>::loop(loopCount);
        TestLoop<MySTLAtomic>::loop(loopCount);
        TestLoop<MutexWrapper>::loop(loopCount);

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
