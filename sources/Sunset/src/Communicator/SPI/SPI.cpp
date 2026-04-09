#include "defines.h"
#include "Communicator/SPI/SPI.h"


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
    static int fd = -1;
    static uint speed = SPI_SPEED;
    static uint8 mode = 0;
    static uint8 bits_per_word = 8;
    static bool gpio_initialized = false;

    const char *device = SPI_DEVICE;

    static const int MAX_DAC_COUNT = 2;

    const char *gpio_chip_name = SPI_CHIP;                                  // Имя GPIO чипа для ARM64
    struct gpiod_chip *gpio_chip = nullptr;                               // Дескриптор GPIO чипа
    struct gpiod_line *g_dac_lines[MAX_DAC_COUNT] = { nullptr, nullptr };   // Линии GPIO для каждого DAC

    const unsigned int cs_DAC[MAX_DAC_COUNT] = {
        0,  // GPIO пин для DAC #1 (pin. 31)
        2   // GPIO пин для DAC #2 (pin. 35)
    };

    // Имена GPIO линий для отладки
    const char *DAC_NAMES[MAX_DAC_COUNT] = {
        "SPI_EN_DDA1",  // Включение DAC #1
        "SPI_EN_DDA2"   // Включение DAC #2
    };

    static bool InitGPIO();
    static void DeInitGPIO();
    static void SetCS(int dac_number, bool enable);   // Управление CS (Chip Select) для конкретного DAC
    static bool Write(uint8 *data, size_t length);

    static bool SetSpeed(uint speedHz);
    // Установка режима SPI (полярность и фаза тактового сигнала)
    // mode: режим SPI (0-3: 0=CPOL=0,CPHA=0; 1=CPOL=0,CPHA=1; 2=CPOL=1,CPHA=0; 3=CPOL=1,CPHA=1)
    static bool SetMode(uint8 mode);

    void Init()
    {
        LOG_WRITE("Initializing SPI...");

        fd = ::open(device, O_RDWR);
        if (fd < 0)
        {
            LOG_ERROR("Cannot open SPI device: %s", device);
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
        {
            LOG_ERROR("Cannot set SPI mode");
            ::close(fd);
            fd = -1;
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0)
        {
            LOG_ERROR("Cannot set bits per word");
            ::close(fd);
            fd = -1;
            return;
        }

        if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        {
            LOG_ERROR("Cannot set SPI speed");
            ::close(fd);
            fd = -1;
            return;
        }

        if (!InitGPIO())
        {
            ::close(fd);
            fd = -1;
            return;
        }

        SetCS(1, true);
        SetCS(2, true);

        SPI::SetSpeed(SPI_SPEED);

        SPI::SetMode(1);        // При такой настройке данные выставляет на срез клока, как нам и нужно

        LOG_WRITE("SPI initialized successfully on %s", device);
    }

    void DeInit()
    {
        if (fd >= 0)
        {
            for (int i = 1; i <= MAX_DAC_COUNT; i++)
            {
                SetCS(i, true);
            }

            ::close(fd);
            fd = -1;
            LOG_WRITE("SPI deinitialized");
        }

        DeInitGPIO();
    }

    // Запись 16-битного значения в динамический DAC через SPI
    // number_DAC: номер DAC (1 или 2)
    // value: 16-битное значение для записи (0x0000-0xFFFF)
    // Возвращает: true если запись успешна, false при ошибке
    bool WriteDynamicDAC(int number_DAC, uint16 value)
    {
        if (!IsReady())
        {
            LOG_ERROR("SPI not ready");
            return false;
        }

        if (number_DAC < 1 || number_DAC > 2)
        {
            LOG_ERROR("Invalid DAC number: %d. Valid range: 1-2");
            return false;
        }

        uint8 data[2];
        data[0] = static_cast<uint8>((value >> 8) & 0xFF);
        data[1] = static_cast<uint8>(value & 0xFF);

        SetCS(number_DAC, false);

        bool result = Write(data, 2);

        SetCS(number_DAC, true);

        return result;
    }

    // Установка скорости SPI интерфейса
    bool SetSpeed(uint speedHz)
    {
        speed = speedHz;

        if (IsReady())
        {
            if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
            {
                LOG_ERROR("Cannot set SPI speed to %u Hz", speedHz);
                return false;
            }

            LOG_WRITE("SPI speed set to %u Hz", speedHz);
        }

        return true;
    }


    bool SetMode(uint8 _mode)
    {
        mode = _mode;

        if (IsReady())
        {
            if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
            {
                LOG_ERROR("Cannot set SPI mode to %d", (int)mode);
                return false;
            }

            LOG_WRITE("SPI mode set to %d", (int)mode);
        }

        return true;
    }

    // Проверка готовности SPI к работе
    // Возвращает: true если SPI устройство открыто и готово к работе, false если закрыто
    bool IsReady()
    {
        return fd >= 0 && gpio_initialized;
    }

    bool InitGPIO()
    {
        if (gpio_initialized)
            return true;

        gpio_chip = gpiod_chip_open_by_name(gpio_chip_name);
        if (!gpio_chip)
        {
            LOG_ERROR("Cannot open %s", gpio_chip_name);
            return false;
        }

        LOG_WRITE("Opened %s  for GPIO3_A0 and GPIO3_A2", gpio_chip_name);

        for (int i = 0; i < MAX_DAC_COUNT; i++)
        {
            unsigned int cs_num = cs_DAC[i];

            LOG_WRITE("Initializing %s (GPIO%u in gpiochip3)", DAC_NAMES[i], cs_num);

            g_dac_lines[i] = gpiod_chip_get_line(gpio_chip, cs_num);
            if (!g_dac_lines[i])
            {
                LOG_ERROR("Cannot get GPIO line %u for %s", cs_num, DAC_NAMES[i]);

                for (int j = 0; j < i; j++) //-V1008
                {
                    if (g_dac_lines[j])
                    {
                        gpiod_line_release(g_dac_lines[j]);
                        g_dac_lines[j] = nullptr;
                    }
                }
                gpiod_chip_close(gpio_chip);
                gpio_chip = nullptr;
                return false;
            }

            if (gpiod_line_request_output(g_dac_lines[i], DAC_NAMES[i], 0) < 0)
            {
                LOG_ERROR("Cannot request GPIO line %s as output", DAC_NAMES[i]);

                for (int j = 0; j <= i; j++)
                {
                    if (g_dac_lines[j])
                    {
                        gpiod_line_release(g_dac_lines[j]);
                        g_dac_lines[j] = nullptr;
                    }
                }
                gpiod_chip_close(gpio_chip);
                gpio_chip = nullptr;
                return false;
            }
        }

        gpio_initialized = true;
        LOG_WRITE("GPIO initialized successfully using libgpiod");
        return true;
    }

    void DeInitGPIO()
    {
        for (int i = 0; i < MAX_DAC_COUNT; i++)
        {
            if (g_dac_lines[i])
            {
                gpiod_line_release(g_dac_lines[i]);
                g_dac_lines[i] = nullptr;
            }
        }

        if (gpio_chip)
        {
            gpiod_chip_close(gpio_chip);
            gpio_chip = nullptr;
        }
        gpio_initialized = false;
    }

    // Внутренняя функция: управление CS (Chip Select) для конкретного DAC
    // dac_number: номер DAC (1 или 2)
    // enable: true = активировать CS (LOW), false = деактивировать CS (HIGH)
    void SetCS(int dac_number, bool enable)
    {
        if (!gpio_initialized)
            return;

        if (dac_number < 1 || dac_number > MAX_DAC_COUNT)
        {
            LOG_ERROR("Invalid DAC number: %d", dac_number);
            return;
        }

        int dac_index = dac_number - 1;
        int value = enable ? 1 : 0;

        if (g_dac_lines[dac_index])
        {
            gpiod_line_set_value(g_dac_lines[dac_index], value);
        }
        else
        {
            LOG_ERROR("GPIO line for DAC%d not initialized", dac_number);
        }
    }

    bool Write(uint8 *data, size_t length)
    {
        if (fd < 0)
        {
            LOG_ERROR("SPI not initialized");
            return false;
        }

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
        transfer.cs_change = 0;

        int result = ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
        if (result < 0)
        {
            LOG_ERROR("SPI transfer failed");
            return false;
        }

        return true;
    }
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
