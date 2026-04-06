#include "ioctl.h"


#ifdef WIN32

int ioctl(int /*fd*/, unsigned long /*request*/, ...)
{
    return 0;
}

#endif
