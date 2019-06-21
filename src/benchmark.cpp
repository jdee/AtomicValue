#include <cerrno>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <sys/stat.h>

#include <AtomicValue/AtomicValue.h>

#include "Metadata.h"
#include "MutexWrapper.h"
#include "MySTLAtomic.h"
#include "timeutil.h"

using namespace AtomicValue;
using namespace std;

#ifdef DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif // DEBUG

#define LOG(m) \
    { \
        ostringstream oss; \
        oss << m; \
        cout << timestamp(currentTime()) << " " << oss.str() << endl; \
    }

void
logBanner()
{
    struct stat sb;
    if (stat("assets/AtomicValue.asc", &sb) && errno == ENOENT)
    {
        LOG("<error rendering ASCII art>");
        return;
    }

    ifstream asciiArt("assets/AtomicValue.asc");
    char line[256];
    while (asciiArt.getline(line, 255))
    {
        LOG(line);
    }
}

template <template <class> class Template>
void
testLoop(unsigned long long maxCount)
{
    typedef Template<unsigned long long> Counter_t;
    Counter_t volatile counter;

    LOG("#####");
    LOG("##### " << Metadata<Template>::testTitle);
    LOG("#####");
    LOG("starting test");

    timeval start(currentTime());
    for (unsigned long long j=0; j<maxCount; ++j)
    {
        ++ counter;
#ifdef DEBUG
        if (j % (maxCount / 100)) continue;
        cout << ".";
        cout.flush();
#endif // DEBUG
    }
    timeval end(currentTime());
#ifdef DEBUG
    cout << endl;
#endif // DEBUG

    auto elapsed(end.tv_sec - start.tv_sec + 1.e-6 * (end.tv_usec - start.tv_usec));
    auto rate(counter / elapsed);

    LOG("done ✅");
    LOG("final counter value: " << counter);
    LOG("time elapsed: " << elapsed << " s");
    LOG("rate: " << rate << "/s");
    LOG("per loop: " << 1.e9/rate << " ns");
}

unsigned long long
getMaxCount(int argc, char** argv)
{
    if (argc < 2) return 10000000000;
    return atoll(argv[1]);
}

int
main(int argc, char** argv)
{
    try
    {
        logBanner();
        LOG("");
        LOG("Build type: " << BUILD_TYPE);

        const unsigned long long maxCount(getMaxCount(argc, argv));

        LOG("loop count: " << maxCount);

        testLoop<FastAtomicReader>(maxCount);
        testLoop<FastAtomicWriter>(maxCount);
        testLoop<MySTLAtomic>(maxCount);
        testLoop<MutexWrapper>(maxCount);

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
