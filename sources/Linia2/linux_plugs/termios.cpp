#include "termios.h"


#ifdef WIN32

int tcflush(int /*fd*/, int /*queue_selector*/)
{
    return 0;
}

int cfsetispeed(struct termios * /*termios_p*/, speed_t /*speed*/)
{
    return 0;
}

int cfsetospeed(struct termios * /*termios_p*/, speed_t /*speed*/)
{
    return 0;
}

int tcsetattr(int /*fd*/, int /*optional_actions*/, const struct termios * /*termios_p*/)
{
    return 0;
}

#endif
