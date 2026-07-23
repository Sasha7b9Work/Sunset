#pragma once


#define SPI_DEVICE     "/dev/spidev0.0"
#define SPI_DEVICE_CS1 "/dev/spidev0.1"
#define SPI_SPEED      1000000
#define SPI_CHIP       "gpiochip3"


namespace SPI
{
    void Init();
    void DeInit();
    bool SetSpeed(uint speedHz);
    bool SetMode(uint8 mode);
    bool IsReady();
    bool IsAvailability();

    // Запись в FPGA
    bool WriteFPGA(int dac_number, uint8 *data, size_t length);

    // Записи в динамические ЦАП
    bool WriteDynamicDAC(int number_DAC, uint16 value);
}
