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
    static int g_spi_fd = -1;
    static uint g_speed = SPI_SPEED;
    static uint8 g_mode = 0;
    static uint8 g_bits_per_word = 8;
    static bool g_gpio_initialized = false;

    const char *device = SPI_DEVICE;

    const int MAX_DAC_COUNT = 2;

    const char *gpio_chip_name = SPI_CHIP;                                  // Имя GPIO чипа для ARM64
    struct gpiod_chip *g_gpio_chip = nullptr;                               // Дескриптор GPIO чипа
    struct gpiod_line *g_dac_lines[MAX_DAC_COUNT] = { nullptr, nullptr };   // Линии GPIO для каждого DAC

    const unsigned int DAC_GPIO_NUMS[MAX_DAC_COUNT] = {
        0,  // GPIO пин для DAC (pin. 31)
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

    void Init()
    {
        LOG_WRITE("Initializing SPI...");

        g_spi_fd = ::open(device, O_RDWR);
        if (g_spi_fd < 0)
        {
            LOG_ERROR("Cannot open SPI device: %s", device);
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_mode) < 0)
        {
            LOG_ERROR("Cannot set SPI mode");
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_BITS_PER_WORD, &g_bits_per_word) < 0)
        {
            LOG_ERROR("Cannot set bits per word");
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (ioctl(g_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &g_speed) < 0)
        {
            LOG_ERROR("Cannot set SPI speed");
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        if (!InitGPIO())
        {
            ::close(g_spi_fd);
            g_spi_fd = -1;
            return;
        }

        LOG_WRITE("SPI initialized successfully on %s", device);
    }

    void DeInit()
    {
        if (g_spi_fd >= 0)
        {
            for (int i = 1; i <= MAX_DAC_COUNT; i++)
            {
                SetCS(i, false);
            }

            ::close(g_spi_fd);
            g_spi_fd = -1;
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

        SetCS(number_DAC, true);
        usleep(1);

        bool result = Write(data, 2);

        if (result)
        {
            LOG_WRITE("DAC%d  written: %04X", number_DAC, value);
        }

        usleep(1);
        SetCS(number_DAC, false);

        return result;
    }

    // Установка скорости SPI интерфейса
    bool SetSpeed(uint speedHz)
    {
        g_speed = speedHz;

        if (IsReady())
        {
            if (ioctl(g_spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &g_speed) < 0)
            {
                LOG_ERROR("Cannot set SPI speed to %u Hz", speedHz);
                return false;
            }

            LOG_WRITE("SPI speed set to %u Hz", speedHz);
        }

        return true;
    }

    // Установка режима SPI (полярность и фаза тактового сигнала)
    // mode: режим SPI (0-3: 0=CPOL=0,CPHA=0; 1=CPOL=0,CPHA=1; 2=CPOL=1,CPHA=0; 3=CPOL=1,CPHA=1)
    // Возвращает: true если режим установлен успешно, false при ошибке
    bool SetMode(uint8 mode)
    {
        g_mode = mode;

        if (IsReady())
        {
            if (ioctl(g_spi_fd, SPI_IOC_WR_MODE, &g_mode) < 0)
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
        return g_spi_fd >= 0 && g_gpio_initialized;
    }

    // Получение текущей скорости SPI
    // Возвращает: скорость в Герцах
    uint GetSpeed()
    {
        return g_speed;
    }

    // Получение текущего режима SPI
    // Возвращает: режим SPI (0-3)
    uint8 GetMode()
    {
        return g_mode;
    }

    bool InitGPIO()
    {
        if (g_gpio_initialized)
            return true;

        g_gpio_chip = gpiod_chip_open_by_name(gpio_chip_name);
        if (!g_gpio_chip)
        {
            LOG_ERROR("Cannot open %s", gpio_chip_name);
            return false;
        }

        LOG_WRITE("Opened %s  for GPIO3_A0 and GPIO3_A2", gpio_chip_name);

        for (int i = 0; i < MAX_DAC_COUNT; i++)
        {
            unsigned int gpio_num = DAC_GPIO_NUMS[i];

            LOG_WRITE("Initializing %s (GPIO%u in gpiochip3)", DAC_NAMES[i], gpio_num);

            g_dac_lines[i] = gpiod_chip_get_line(g_gpio_chip, gpio_num);
            if (!g_dac_lines[i])
            {
                LOG_ERROR("Cannot get GPIO line %u for %s", gpio_num, DAC_NAMES[i]);

                for (int j = 0; j < i; j++) //-V1008
                {
                    if (g_dac_lines[j])
                    {
                        gpiod_line_release(g_dac_lines[j]);
                        g_dac_lines[j] = nullptr;
                    }
                }
                gpiod_chip_close(g_gpio_chip);
                g_gpio_chip = nullptr;
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
                gpiod_chip_close(g_gpio_chip);
                g_gpio_chip = nullptr;
                return false;
            }
        }

        g_gpio_initialized = true;
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

        if (g_gpio_chip)
        {
            gpiod_chip_close(g_gpio_chip);
            g_gpio_chip = nullptr;
        }
        g_gpio_initialized = false;
    }

    // Внутренняя функция: управление CS (Chip Select) для конкретного DAC
    // dac_number: номер DAC (1 или 2)
    // enable: true = активировать CS (LOW), false = деактивировать CS (HIGH)
    void SetCS(int dac_number, bool enable)
    {
        if (!g_gpio_initialized)
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
        if (g_spi_fd < 0)
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
        transfer.speed_hz = g_speed;
        transfer.delay_usecs = 0;
        transfer.bits_per_word = g_bits_per_word;
        transfer.cs_change = 0;

        int result = ioctl(g_spi_fd, SPI_IOC_MESSAGE(1), &transfer);
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
