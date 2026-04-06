#pragma once


#ifndef ARM64

#define SPI_IOC_WR_MODE             0UL
#define SPI_IOC_WR_BITS_PER_WORD    0UL
#define SPI_IOC_WR_MAX_SPEED_HZ     0UL

#define SPI_IOC_MESSAGE(N) N

struct spi_ioc_transfer
{
    unsigned long long tx_buf;              // Указатель на данные для передачи
    unsigned long long rx_buf;              // Указатель на буфер для приёма
    unsigned int len;                 // Длина данных в байтах
    unsigned int speed_hz;            // Скорость передачи (можно переопределить)
    unsigned short delay_usecs;         // Задержка после передачи
    unsigned char bits_per_word;        // Битов на слово (можно переопределить)
    unsigned char cs_change;            // Изменение состояния chip select
//    __u8 tx_nbits;             // Количество линий для передачи (для dual/quad SPI)
//    __u8 rx_nbits;             // Количество линий для приёма (для dual/quad SPI)
//    __u8 pad;                  // Выравнивание
};

#endif
