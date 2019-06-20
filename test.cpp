#include <cstdlib>
#include <exception>
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

unsigned long long
getMaxCount(int argc, char** argv)
{
    if (argc < 2) return 10000000000;
    return atoi(argv[1]);
}

int
main(int argc, char** argv)
{
    try
    {
        FastAtomicReader<unsigned long long> counter;

        cout << "Build type: " << BUILD_TYPE << endl;

        const unsigned long long maxCount(getMaxCount(argc, argv));
        cout << "loop count: " << maxCount << endl;
        cout << "starting test" << endl;

        timeval start;
        gettimeofday(&start, NULL);
        for (unsigned long long j=0; j<maxCount; ++j)
        {
            ++ counter;
#ifdef VERBOSE
            if (j % (maxCount / 100)) continue;
            cout << ".";
            cout.flush();
#endif // VERBOSE
        }
        timeval end;
        gettimeofday(&end, NULL);
#ifdef VERBOSE
        cout << endl;
#endif // VERBOSE

        auto elapsed(end.tv_sec - start.tv_sec + 1.e-6 * (end.tv_usec - start.tv_usec));
        auto rate(counter / elapsed);
        cout << "done ✅" << endl;

        cout << "final counter value: " << counter << endl;
        cout << "time elapsed: " << elapsed << " s" << endl;
        cout << "rate: " << rate << "/s" << endl;
        cout << "per loop: " << 1.e9/rate << " ns" << endl;

        return 0;
    }
    catch (exception& e)
    {
        cerr << "Exception in main: " << e.what() << endl;
        return -1;
    }
    catch (...)
    {
        cerr << "Unexpected exception in main. Aborting." << endl;
        abort();
    }
}
