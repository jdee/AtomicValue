#ifndef __TESTLOOP_H__
#define __TESTLOOP_H__

#include <cassert>

#include "Accumulator.h"
#include "Metadata.h"
#include "log.h"
#include "static_assert.h"
#include "timeutil.h"

#ifdef VOLATILE_COUNTERS
#define VOLATILE volatile
#else
#define VOLATILE
#endif // VOLATILE_COUNTERS

template <template <class> class Template, unsigned int iterations = 100>
struct
TestLoop
{
    typedef Template<unsigned long long> Counter_t;

    static void loop(unsigned long long loopCount)
    {
        LOG("#####");
        LOG("##### " << Metadata<Template>::testTitle);
        LOG("#####");
        LOG("starting test (" << iterations << " x " << loopCount << ")");

        Accumulator accumulator;
        while (accumulator.count() < iterations)
        {
            accumulator << runIteration(loopCount);
        }

        LOG("done ✅");
        report(accumulator, loopCount);
    }

protected:
    static double runIteration(unsigned long long loopCount)
    {
        Counter_t VOLATILE counter;
        auto const start(currentTime());
        for (unsigned long long j=0; j<loopCount; ++j)
        {
            ++ counter;
        }
        auto const end(currentTime());
        return end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / UsPerSec;
    }

    static void report(const Accumulator& accumulator, unsigned long long loopCount)
    {
        auto const totalTime(accumulator.total());
        LOG("time elapsed: " << MsPerSec * totalTime << " ms");

        if (totalTime <= 0 || loopCount == 0)
        {
            return;
        }

        // ----- statistical errors -----

        // mean time per iteration
        auto const meanTime(accumulator.mean());
        assert(meanTime > 0);

        // Can't compute SD without at least 2 iterations because of the
        // denominator (accumulator.sigma() will return NaN).
        STATIC_ASSERT(iterations > 1, iterations_must_be_gt_1);
        assert(accumulator.count() == iterations);

        // everything scales to the mean time
        auto const relativeSigma(accumulator.sigma() / meanTime);
        // error on mean time
        auto const relativeError(accumulator.error() / meanTime);

        auto const totalLoops(iterations * loopCount);
        assert(totalLoops > 0);

        // period in ns
        auto const meanPeriod(NsPerSec*totalTime/totalLoops);
        auto const periodError(meanPeriod * relativeError);
        auto const periodSigma(meanPeriod * relativeSigma);

        // rate in MHz
        assert(meanPeriod > 0);
        auto const meanRate(1.e3/meanPeriod);
        auto const rateError(meanRate * relativeError);
        auto const rateSigma(meanRate * relativeSigma);

        LOG("rate: " << meanRate << " ± " << rateError << " MHz (σ = " << rateSigma << " MHz)");
        LOG("per loop: " << meanPeriod << " ± " << periodError << " ns (σ = " << periodSigma << " ns)");
    }
};

#endif // __TESTLOOP_H__
