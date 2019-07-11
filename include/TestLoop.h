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
    typedef Template<unsigned long long> Counter_t;

    static void loop(unsigned int numLoops, unsigned long long loopCount)
    {
        LOG("#####");
        LOG("##### " << Metadata<Template>::testTitle);
        LOG("#####");
        LOG("starting test (" << numLoops << " x " << loopCount << ")");

        double total(0);
        double totalSquare(0);
        for (unsigned int i=0; i<numLoops; ++i)
        {
            auto const start(currentTime());
            run(loopCount);
            auto const end(currentTime());
            auto const elapsed(end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / UsPerSec);
            total += elapsed;
            totalSquare += elapsed * elapsed;
        }

        LOG("done ✅");
        LOG("time elapsed: " << total << " s");
        if (total > 0)
        {
            auto const totalLoops(numLoops * loopCount);
            auto const rate(totalLoops / total);
            // statistical errors
            auto const meanTime(total / numLoops);
            auto const rmsTime(sqrt(totalSquare / numLoops - meanTime * meanTime));
            auto const relativeError(rmsTime/meanTime);
            auto const rateError(rate * relativeError);
            auto const period(1./rate);
            auto const periodError(period * relativeError);

            LOG("rate: " << rate << " ± " << rateError << "/s");
            LOG("per loop: " << NsPerSec * period << " ± " << NsPerSec * periodError << " ns");
        }
    }

protected:
    static void run(unsigned long long maxCount)
    {
        Counter_t VOLATILE counter;

        for (unsigned long long j=0; j<maxCount; ++j)
        {
            ++ counter;
#ifdef DEBUG
            if (j % (maxCount / 100)) continue;
            cout << ".";
            cout.flush();
#endif // DEBUG
        }
#ifdef DEBUG
        cout << endl;
#endif // DEBUG
    }
};

#endif // __TESTLOOP_H__
