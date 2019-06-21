#include <cstring>

#include "timeutil.h"

std::string
timestamp(const timeval& tv)
{
    time_t epochTime(tv.tv_sec);
    tm* tm_time = localtime(&epochTime);
    // YYYY-MM-DD-hh:mm:ss.dddddd
    char buffer[32];
    strftime(buffer, 20, "%Y-%m-%d-%H:%M:%S", tm_time);

    char decimal[8];
    sprintf(decimal, ".%06ld", tv.tv_usec);
    strcat(buffer, decimal);
    return buffer;
}
