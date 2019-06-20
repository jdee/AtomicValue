#include <iostream>
#include <sys/time.h>

#include <AtomicValue/AtomicValue.h>

using namespace AtomicValue;
using namespace std;

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif // DEBUG

int
main(int argc, char** argv)
{
    FastAtomicReader<unsigned long long> counter;

    timeval start;
    gettimeofday(&start, NULL);

    cout << "Build type: " << BUILD_TYPE << endl;

    const unsigned long long maxCount(10000000000);
    for (unsigned long long j=0; j<maxCount; ++j)
    {
        ++ counter;
        if (j % (maxCount / 100)) continue;
        cout << ".";
        cout.flush();
    }
    timeval end;
    gettimeofday(&end, NULL);
    cout << endl;

    double elapsed(end.tv_sec - start.tv_sec + 1.e-6 * (end.tv_usec - start.tv_usec));
    double rate(counter / elapsed);

    cout << "counter: " << counter << endl;
    cout << "time elapsed: " << elapsed << " s" << endl;
    cout << "rate: " << rate << "/s" << endl;
    cout << "per loop: " << 1.e9/rate << " ns" << endl;

    return 0;
}
