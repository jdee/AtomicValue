#ifndef __TIMEUTIL_H__
#define __TIMEUTIL_H__

#include <ctime>
#include <string>
#include <sys/time.h>

inline timeval
currentTime()
{
    timeval now;
    gettimeofday(&now, NULL);
    return now;
}

std::string
timestamp(const timeval& tv);

#endif // __TIMEUTIL_H__
