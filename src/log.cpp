#include <cerrno>
#include <fstream>

#include <sys/stat.h>

#include "log.h"

void
logBanner()
{
    struct stat sb;
    if (stat("assets/AtomicValue.asc", &sb) && errno == ENOENT)
    {
        LOG("<error rendering ASCII art>");
        return;
    }

    std::ifstream asciiArt("assets/AtomicValue.asc");
    char line[256];
    while (asciiArt.getline(line, 255))
    {
        LOG(line);
    }
}
