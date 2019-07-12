#ifndef __TESTLOOP_H__
#define __TESTLOOP_H__

#include <cassert>
#include <cmath>

#include "Metadata.h"
#include "log.h"
#include "static_assert.h"
#include "timeutil.h"

#ifdef VOLATILE_COUNTERS
#define VOLATILE volatile
#else
#define VOLATILE
#endif // VOLATILE_COUNTERS

template <template <class> class Template, unsigned int iterations>
struct
TestLoop
{
    typedef Template<unsigned long long> Counter_t;

    static void loop(unsigned long long totalLoops)
    {
        assert(totalLoops > 0 && totalLoops % iterations == 0);
        auto const loopCount(totalLoops/iterations);

        LOG("#####");
        LOG("##### " << Metadata<Template>::testTitle);
        LOG("#####");
        LOG("starting test (" << iterations << " x " << loopCount << ")");

        double totalTime(0);
        double totalSquare(0);
        for (unsigned int i=0; i<iterations; ++i)
        {
            auto const elapsed(runIteration(loopCount));
            totalTime += elapsed;
            totalSquare += elapsed * elapsed;
        }

        LOG("done ✅");
        LOG("time elapsed: " << totalTime << " s");
        if (totalTime > 0)
        {
            // ----- statistical errors -----

            // mean time per iteration
            auto const meanTime(totalTime / iterations);
            // RMS deviation
            auto const rmsTime(sqrt(totalSquare / iterations - meanTime * meanTime));

            // Can't compute SD without at least 2 iterations because of the
            // denominator on the next line.
            STATIC_ASSERT(iterations > 1, iterations_must_be_gt_1);

            // SD/RMS
            auto const sdRatio(sqrt(1.*iterations/(iterations-1)));
            // standard deviation
            auto const sigma(sdRatio * rmsTime);
            // everything scales to the mean time
            auto const relativeSigma(sigma/meanTime);
            // error on mean time
            auto const relativeError(rmsTime/meanTime/sqrt(1. * iterations));

            auto const meanRate(totalLoops / totalTime);
            auto const rateError(meanRate * relativeError);
            auto const rateSigma(meanRate * relativeSigma);

            auto const meanPeriod(1./meanRate);
            auto const periodError(meanPeriod * relativeError);
            auto const periodSigma(meanPeriod * relativeSigma);

            LOG("rate: " << meanRate << " ± " << rateError << " Hz (σ = " << rateSigma << " Hz)");
            LOG("per loop: " << NsPerSec * meanPeriod << " ± " << NsPerSec * periodError << " ns (σ = " << NsPerSec * periodSigma << " ns)");
        }
    }

protected:
    static double runIteration(unsigned long long maxCount)
    {
        Counter_t VOLATILE counter;
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
        return end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / UsPerSec;
    }
};

#endif // __TESTLOOP_H__
