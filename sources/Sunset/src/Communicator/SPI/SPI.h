#pragma once


#define SPI_DEVICE     "/dev/spidev0.0"
#define SPI_DEVICE_CS1 "/dev/spidev0.1"
#define SPI_SPEED      1000000
#define SPI_CHIP       "gpiochip3"


// \todo Для SPI нужно использовать 24 SPI0_CS0_M2 и 26 SPI0_CS1_M2.
// Программные чипселекты слишком долго выполняются

namespace SPI
{
    void Init();
    void DeInit();
    bool WriteDynamicDAC(int number_DAC, uint16 value);
    bool SetSpeed(uint speedHz);
    bool SetMode(uint8 mode);
    bool IsReady();
    bool IsAvailability();

    // Добавить новую функцию для отправки произвольных данных
    bool WriteRaw(int dac_number, uint8 *data, size_t length);
}
