#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <exception>

#include <AtomicValue/AtomicValue.h>

#include "log.h"
#include "Metadata.h"
#include "MutexWrapper.h"
#include "MySTLAtomic.h"

using namespace AtomicValue;
using namespace std;

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif // DEBUG

#ifdef VOLATILE_COUNTERS
#define VOLATILE volatile
#else
#define VOLATILE
#endif // VOLATILE_COUNTER

template <template <class> class Template>
struct
TestLoop
{
    static constexpr auto const NsPerSec = 1.e9;
    static constexpr auto const UsPerSec = 1.e6;

    static void run(unsigned long long maxCount)
    {
        typedef Template<unsigned long long> Counter_t;
        Counter_t VOLATILE counter;

        LOG("#####");
        LOG("##### " << Metadata<Template>::testTitle);
        LOG("#####");
        LOG("starting test");

        timeval const start(currentTime());
        for (unsigned long long j=0; j<maxCount; ++j)
        {
            ++ counter;
#ifdef DEBUG
            if (j % (maxCount / 100)) continue;
            cout << ".";
            cout.flush();
#endif // DEBUG
        }
        timeval const end(currentTime());
#ifdef DEBUG
        cout << endl;
#endif // DEBUG

        auto const elapsed(end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / UsPerSec);
        auto const rate(counter / elapsed);

        // statistical errors
        auto const rateError(sqrt(1. * counter) / elapsed);
        auto const periodError(NsPerSec * rateError / rate / rate);

        LOG("done ✅");
        LOG("final counter value: " << counter);
        LOG("time elapsed: " << elapsed << " s");
        if (isfinite(rate))
        {
            LOG("rate: " << rate << " ± " << rateError << "/s");
            LOG("per loop: " << NsPerSec/rate << " ± " << periodError << " ns");
        }
    }
};

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
