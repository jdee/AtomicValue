#include <cstring>
#include <sys/time.h>

#include "timeutil.h"

using namespace std;

string
timestamp(const timeval& tv)
{
    time_t epochTime(tv.tv_sec);
    tm tm_time;
    localtime_r(&epochTime, &tm_time);
    // YYYY-MM-DD-hh:mm:ss.dddddd
    char buffer[32];
    strftime(buffer, 20, "%Y-%m-%d-%H:%M:%S", &tm_time);

    char decimal[8];
    snprintf(decimal, 8, ".%06ld", long(tv.tv_usec));
    strcat(buffer, decimal);
    return buffer;
}

timeval
currentTime()
{
    timeval now;
    gettimeofday(&now, NULL);
    return now;
}
