#include "defines.h"
#include "Communicator/UART/UART.h"
#include <filesystem>


//  Lin specific
#include <unistd.h>
#include <sys/file.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/ioctl.h>


#ifdef WIN32
    #pragma warning(push)
    #pragma warning(disable:4365 4389 4996)
    #define O_NOCTTY  0x00000400
    #define O_NDELAY  0
#endif


namespace UART
{
    static int fd = -1;

    static void (*recv_callback)(uint8 *, int) = nullptr;

    static pthread_t id_thread = (pthread_t)-1;
    static bool need_stop_reading = false;          // С помощью этого флага будем останавливать поток

    static int GetBaudrateConstant(int baudrate);
    static bool ConfigurePort();
    static bool Open();
    static void Close();
    static bool IsReady();

    // Функция потока чтения UART
    // Читает данные из UART и вызывает recv_callback для каждого принятого байта
    static void *ReaderThreadFunc(void *arg);
}


bool UART::Init(void (*callback)(uint8 *, int))
{
    fd = -1;
    recv_callback = callback;
    id_thread = (pthread_t)-1;

    return Open();
}


void UART::ReInit()
{
    DeInit();

    Init(recv_callback);
}


void UART::DeInit()
{
    if (IsReady())
    {
        Close();
    }
}


bool UART::Open()
{
    if (IsReady())
    {
        LOG_ERROR("UART already opened");
        return false;
    }

    fd = ::open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        LOG_ERROR("Cannot open UART device: %s", UART_DEVICE);
        return false;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) != 0)
    {
        LOG_ERROR("Cannot lock UART device");
        ::close(fd);
        fd = -1;
        return false;
    }

    if (!ConfigurePort())
    {
        LOG_ERROR("Cannot configure UART port");
        flock(fd, LOCK_UN);
        ::close(fd);
        fd = -1;
        return false;
    }

    need_stop_reading = false;
    if (pthread_create(&id_thread, nullptr, ReaderThreadFunc, nullptr) != 0)
    {
        LOG_ERROR("Cannot create reader thread");
        flock(fd, LOCK_UN);
        ::close(fd);
        fd = -1;
        return false;
    }

    LOG_WRITE("UART opened successfully on %s with baudrate %d and mode %s", UART_DEVICE, UART_BAUDRATE, UART_MODE);
    return true;
}


void UART::Close()
{
    if (!IsReady()) return;

    need_stop_reading = true;
    if (id_thread != (pthread_t)-1)
    {
        pthread_join(id_thread, nullptr);
        id_thread = (pthread_t)-1;
    }

    int status;
    if (ioctl(fd, TIOCMGET, &status) != -1)
    {
        status &= ~(TIOCM_DTR | TIOCM_RTS);
        ioctl(fd, TIOCMSET, &status);
    }

    flock(fd, LOCK_UN);
    ::close(fd);
    fd = -1;

    LOG_WRITE("UART closed");
}


bool UART::SendByte(uint8 byte)
{
    if (!IsReady())
    {
        LOG_ERROR("UART not ready");
        return false;
    }

    int n = write(fd, &byte, 1);
    if (n < 0)
    {
        LOG_ERROR("Failed to send byte");
        return false;
    }

    return (n == 1);
}


bool UART::SendBuffer(const void *_buffer, int size)
{
    if (!IsReady())
    {
        LOG_ERROR("UART not ready");
        return false;
    }

    const uint8 *buffer = (const uint8 *)_buffer;

    if (buffer == nullptr || size <= 0)
    {
        LOG_ERROR("Invalid buffer or size");
        return false;
    }

    int n = write(fd, buffer, size);

    if (n < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            LOG_ERROR("Failed to send buffer, errno: %d", errno);
            return false;
        }
    }

    return (n == size);
}


void UART::Flush()
{
    if (IsReady())
    {
        tcflush(fd, TCIOFLUSH);
    }
}


bool UART::IsReady()
{
    return fd >= 0;
}


int UART::GetBaudrateConstant(int baudrate)
{
    switch (baudrate)
    {
    case 9600:   return B9600;
    case 19200:  return B19200;
    case 38400:  return B38400;
    case 57600:  return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 460800: return B460800;
    case 921600: return B921600;
    default:     return -1;
    }
}


bool UART::ConfigurePort()
{
    struct termios port_settings;
    memset(&port_settings, 0, sizeof(port_settings));

    int baudr = GetBaudrateConstant(UART_BAUDRATE);
    if (baudr == -1)
    {
        LOG_ERROR("Unsupported baudrate: %d", UART_BAUDRATE);
        return false;
    }

    int cbits = CS8;
    int cpar = 0;
    int ipar = IGNPAR;
    int bstop = 0;

    switch (UART_MODE[0])
    {
    case '8': cbits = CS8; break; //-V1048
    case '7': cbits = CS7; break;
    case '6': cbits = CS6; break;
    case '5': cbits = CS5; break;
    default: cbits = CS8; break; //-V1048
    }

    switch (UART_MODE[1])
    {
    case 'N': case 'n': cpar = 0; ipar = IGNPAR; break; //-V1048 //-V525
    case 'E': case 'e': cpar = PARENB; ipar = INPCK; break;
    case 'O': case 'o': cpar = (PARENB | PARODD); ipar = INPCK; break;
    default: cpar = 0; ipar = IGNPAR; break; //-V1048
    }

    switch (UART_MODE[2])
    {
    case '1': bstop = 0; break;
    case '2': bstop = CSTOPB; break;
    default: bstop = 0; break;
    }

    port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
    port_settings.c_iflag = ipar;
    port_settings.c_oflag = 0;
    port_settings.c_lflag = 0;
    port_settings.c_cc[VMIN] = 0;
    port_settings.c_cc[VTIME] = 0;

    cfsetispeed(&port_settings, baudr);
    cfsetospeed(&port_settings, baudr);

    if (tcsetattr(fd, TCSANOW, &port_settings) == -1)
    {
        LOG_ERROR("Cannot set port settings");
        return false;
    }

    int status;
    if (ioctl(fd, TIOCMGET, &status) == -1)
    {
        LOG_ERROR("Cannot get modem status");
        return false;
    }

    status |= TIOCM_DTR | TIOCM_RTS;

    if (ioctl(fd, TIOCMSET, &status) == -1)
    {
        LOG_ERROR("Cannot set modem status");
        return false;
    }

    return true;
}


void *UART::ReaderThreadFunc(void *)
{
    uint8 buffer[1024];
    fd_set read_fds;
    struct timeval timeout;

    while (!need_stop_reading)
    {
        // Настраиваем select для ожидания данных
        FD_ZERO(&read_fds);
#ifdef WIN32
    #pragma warning(push, 0)
#endif
        FD_SET(fd, &read_fds);
#ifdef WIN32
    #pragma warning(pop)
#endif
        timeout.tv_sec = 0;
        timeout.tv_usec = 10000;

        // select блокируется до появления данных или таймаута
        int result = select(fd + 1, &read_fds, nullptr, nullptr, &timeout);

        if (result > 0 && FD_ISSET(fd, &read_fds))
        {
            int bytes_read = read(fd, buffer, sizeof(buffer));

            if (bytes_read > 0)
            {
                recv_callback(buffer, bytes_read);
            }
            else if (bytes_read < 0)
            {
                if (errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    LOG_ERROR("Read failed");
                    break;
                }
            }
        }
        else if (result < 0)
        {
            if (errno != EINTR)
            {
                LOG_ERROR("Select failed");
                break;
            }
        }

    }

    return nullptr;
}


bool UART::IsAvailability()
{
#ifdef ARM64
    return std::filesystem::exists(UART_DEVICE);
#else
    return true;
#endif
}


#ifdef WIN32
    #pragma warning(pop)
#endif
