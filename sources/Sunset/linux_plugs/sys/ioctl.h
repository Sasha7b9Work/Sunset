#pragma once


#ifdef WIN32

#define TIOCMGET  0
#define TIOCM_DTR 0
#define TIOCM_RTS 0
#define TIOCMSET  0

int ioctl(int fd, unsigned long request, ...); //-V1071

#endif
