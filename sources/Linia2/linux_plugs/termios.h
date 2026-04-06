#pragma once


#ifdef WIN32

#define TCIOFLUSH   2
#define B9600       0000015
#define B19200      0
#define B38400      0
#define B57600      0
#define B115200     0
#define B230400     0
#define B460800     0
#define B921600     0

#define NCCS 19

#define CS5 0000060
#define CS6 0000061
#define CS7 0000062
#define CS8 0000063

#define IGNPAR 0000004
#define PARENB 0000400
#define INPCK  0000020
#define PARODD 0001000
#define CSTOPB 0000100

#define CLOCAL 0
#define CREAD  0
#define VMIN   0
#define VTIME  1

#define TCSANOW   0

typedef unsigned int tcflag_t;
typedef unsigned char cc_t;
typedef unsigned int speed_t;

struct termios
{
    tcflag_t c_iflag;  // Input modes (флаги ввода)
    tcflag_t c_oflag;  // Output modes (флаги вывода)
    tcflag_t c_cflag;  // Control modes (управл€ющие флаги)
    tcflag_t c_lflag;  // Local modes (локальные флаги)
    cc_t     c_cc[NCCS]; // Control characters (управл€ющие символы)
};

int tcflush(int fd, int queue_selector);
int cfsetispeed(struct termios *termios_p, speed_t speed);
int cfsetospeed(struct termios *termios_p, speed_t speed);
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);

#endif
