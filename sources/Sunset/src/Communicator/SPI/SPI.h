#pragma once


#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_SPEED  1000000
#define SPI_CHIP   "gpiochip3"


namespace SPI
{
    void Init();
    void DeInit();
    bool IsAvailability();
    bool IsReady();

    // Запись в FPGA
    bool WriteFPGA(uint8 *data, size_t length);

    // Чтение из FPGA
    bool ReadFPGA(uint8 *data, size_t length);

    // Записи в динамические ЦАП
    bool WriteDynamicDAC(int number_DAC, uint16 value);
}
