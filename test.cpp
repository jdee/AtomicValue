#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <sstream>
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

string
timestamp(const timeval& tv)
{
    tm tm_time;
    localtime_r(&tv.tv_sec, &tm_time);
    // YYYY-MM-DD-hh:mm:ss.dddddd
    char buffer[32];
    strftime(buffer, 20, "%Y-%m-%d-%H:%M:%S", &tm_time);

    char decimal[8];
    sprintf(decimal, ".%06d", tv.tv_usec);
    strcat(buffer, decimal);
    return buffer;
}

int
main(int argc, char** argv)
{
    try
    {
        FastAtomicReader<unsigned long long> volatile counter;

        cout << "Build type: " << BUILD_TYPE << endl;

        const unsigned long long maxCount(getMaxCount(argc, argv));
        cout << "loop count: " << maxCount << endl;

        timeval start;
        gettimeofday(&start, NULL);
        cout << timestamp(start) << " starting test" << endl;

        for (unsigned long long j=0; j<maxCount; ++j)
        {
            ++ counter;
#ifdef DEBUG
            if (j % (maxCount / 100)) continue;
            cout << ".";
            cout.flush();
#endif // DEBUG
        }
        timeval end;
        gettimeofday(&end, NULL);
#ifdef DEBUG
        cout << endl;
#endif // DEBUG

        auto elapsed(end.tv_sec - start.tv_sec + 1.e-6 * (end.tv_usec - start.tv_usec));
        auto rate(counter / elapsed);
        cout << timestamp(end) << " done ✅" << endl;

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
