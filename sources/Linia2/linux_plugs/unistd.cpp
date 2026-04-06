#include "unistd.h"


#ifdef WIN32

int usleep(int /*usec*/)
{
    return 0;
}

#endif
