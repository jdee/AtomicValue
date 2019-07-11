#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <sstream>

#include "timeutil.h"

#define LOG(m) \
    { \
        std::ostringstream oss; \
        oss << m; \
        std::cout << timestamp(currentTime()) << " " << oss.str() << std::endl; \
    }

extern void logBanner();

#endif // __LOG_H__
