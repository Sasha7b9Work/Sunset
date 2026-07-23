#include "defines.h"
#include "Communicator/SPI/SPI.h"
#include <filesystem>


// Lin specific
#include <gpiod.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace SPI
{
    static int fd_cs0 = -1;  // для CS0 (пин 24)
    static uint speed = SPI_SPEED;
    static uint8 mode = 0;
    static uint8 bits_per_word = 8;

    const char *device_cs0 = SPI_DEVICE;      // "/dev/spidev0.0"

    static bool SetSpeed(uint speedHz);
    static bool SetMode(uint8 mode);
    static bool Write(uint8 *data, size_t length);
}

void SPI::Init()
{
    LOG_WRITE("Initializing SPI with hardware CS...");

    fd_cs0 = ::open(device_cs0, O_RDWR);
    if (fd_cs0 < 0)
    {
        LOG_ERROR("Cannot open SPI device %s", device_cs0);
    }
    else
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MODE, &mode) < 0 ||
            ioctl(fd_cs0, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0 ||
            ioctl(fd_cs0, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot configure CS0");
            ::close(fd_cs0);
            fd_cs0 = -1;
        }
    }

    if (fd_cs0 >= 0) LOG_WRITE("SPI CS0 (pin 24) initialized");

    SPI::SetSpeed(SPI_SPEED);
    SPI::SetMode(1);

    LOG_WRITE("SPI initialized successfully");
}

void SPI::DeInit()
{
    if (fd_cs0 >= 0)
    {
        ::close(fd_cs0);
        fd_cs0 = -1;
    }

    LOG_WRITE("SPI deinitialized");
}

bool SPI::WriteDynamicDAC(int /*number_DAC*/, uint16 /*value*/)
{
    return false;
}

bool SPI::Write(uint8 *data, size_t length)
{
    if (data == nullptr || length == 0)
    {
        LOG_ERROR("Invalid data or length");
        return false;
    }

    struct spi_ioc_transfer transfer = {};
    transfer.tx_buf = (unsigned long long)data;
    transfer.rx_buf = 0;
    transfer.len = (uint)length;
    transfer.speed_hz = speed;
    transfer.delay_usecs = 0;
    transfer.bits_per_word = bits_per_word;
    transfer.cs_change = 0;                 // 0 - CS деактивируется после передачи

    int result = ioctl(fd_cs0, SPI_IOC_MESSAGE(1), &transfer);
    if (result < 0)
    {
        LOG_ERROR("SPI transfer failed");
        return false;
    }

    return true;
}


bool SPI::WriteFPGA(uint8 *data, size_t length)
{
    if (data == nullptr || length == 0)
    {
        LOG_ERROR("Invalid data or length");
        return false;
    }

    if (fd_cs0 < 0)
    {
        LOG_ERROR("SPI not initialized");
        return false;
    }

    return Write(data, length);
}


bool SPI::ReadFPGA(uint8 *data, size_t length)
{
    if (data == nullptr || length == 0)
    {
        LOG_ERROR("Invalid data or length");
        return false;
    }

    if (fd_cs0 < 0)
    {
        LOG_ERROR("SPI device not initialized");
        return false;
    }

    // Создаем буфер для отправки (обычно нули или 0xFF для чтения)
    // При SPI чтении мы одновременно отправляем данные (обычно 0x00)
    uint8 *tx_buffer = new uint8[length]();
    if (tx_buffer == nullptr)
    {
        LOG_ERROR("Failed to allocate TX buffer");
        return false;
    }

    // Заполняем нулями (можно заменить на 0xFF если нужно)
    memset(tx_buffer, 0x00, length);

    struct spi_ioc_transfer transfer = {};
    transfer.tx_buf = (unsigned long long)tx_buffer;
    transfer.rx_buf = (unsigned long long)data;
    transfer.len = (uint)length;
    transfer.speed_hz = speed;
    transfer.delay_usecs = 0;
    transfer.bits_per_word = bits_per_word;
    transfer.cs_change = 0;                 // 0 - CS деактивируется после передачи

    int result = ioctl(fd_cs0, SPI_IOC_MESSAGE(1), &transfer);

    delete[] tx_buffer;

    if (result < 0)
    {
        LOG_ERROR("SPI read failed");
        return false;
    }

    return true;
}


bool SPI::SetSpeed(uint speedHz)
{
    speed = speedHz;
    bool success = true;

    if (fd_cs0 >= 0)
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set speed for CS0");
            success = false;
        }
    }

    if (success) LOG_WRITE("SPI speed set to %u Hz", speedHz);
    return success;
}

bool SPI::SetMode(uint8 _mode)
{
    mode = _mode;
    bool success = true;

    if (fd_cs0 >= 0)
    {
        if (ioctl(fd_cs0, SPI_IOC_WR_MODE, &mode) < 0)
        {
            LOG_ERROR("Cannot set mode for CS0");
            success = false;
        }
    }

    if (success) LOG_WRITE("SPI mode set to %d", (int)mode);
    return success;
}


bool SPI::IsReady()
{
    return (fd_cs0 >= 0);
}


bool SPI::IsAvailability()
{
#ifdef ARM64
    return std::filesystem::exists(SPI_DEVICE);
#else
    return true;
#endif
}


#ifdef WIN32
#pragma warning(pop)
#endif
