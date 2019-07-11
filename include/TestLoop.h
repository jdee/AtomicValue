#ifndef __TESTLOOP_H__
#define __TESTLOOP_H__

#include <cmath>

#include "Metadata.h"
#include "log.h"
#include "timeutil.h"

#ifdef VOLATILE_COUNTERS
#define VOLATILE volatile
#else
#define VOLATILE
#endif // VOLATILE_COUNTERS

template <template <class> class Template>
struct
TestLoop
{
    static void run(unsigned long long maxCount)
    {
        typedef Template<unsigned long long> Counter_t;
        Counter_t VOLATILE counter;

        LOG("#####");
        LOG("##### " << Metadata<Template>::testTitle);
        LOG("#####");
        LOG("starting test");

        auto const start(currentTime());
        for (unsigned long long j=0; j<maxCount; ++j)
        {
            ++ counter;
#ifdef DEBUG
            if (j % (maxCount / 100)) continue;
            cout << ".";
            cout.flush();
#endif // DEBUG
        }
        auto const end(currentTime());
#ifdef DEBUG
        cout << endl;
#endif // DEBUG

        auto const elapsed(end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / UsPerSec);
        auto const rate(counter / elapsed);

        LOG("done ✅");
        LOG("final counter value: " << counter);
        LOG("time elapsed: " << elapsed << " s");
        if (elapsed > 0)
        {
            // statistical errors
            auto const rateError(sqrt(1. * counter) / elapsed);
            auto const periodError(NsPerSec * rateError / rate / rate);

            LOG("rate: " << rate << " ± " << rateError << "/s");
            LOG("per loop: " << NsPerSec/rate << " ± " << periodError << " ns");
        }
    }
};

#endif // __TESTLOOP_H__
