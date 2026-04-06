#pragma once


#ifdef WIN32

int flock(int fd, int operation);

#define LOCK_EX 2
#define LOCK_NB 0
#define LOCK_UN 0

#endif
