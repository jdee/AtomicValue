#ifndef __TIMEUTIL_H__
#define __TIMEUTIL_H__

#include <ctime>
#include <string>

extern timeval
currentTime();

extern std::string
timestamp(const timeval& tv);

constexpr auto const NsPerSec = 1.e9;
constexpr auto const UsPerSec = 1.e6;

#endif // __TIMEUTIL_H__
