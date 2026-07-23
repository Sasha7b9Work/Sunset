#pragma once
#include "Communicator/GPIO/PinInfo.h"


/*
          Стало     Было          Стало    Было
     1                       21 SPI_MISO   STOP
     2                       22 F_CON2     DAT_F2
     3 T13                   23 SPI_CLK    SPI1_CLK
     4                       24 SPICS      DAT_F3
     5 T14                   25 GND
     6 GND                   26 F_CON1     SCICS
     7 T15                   27 ENB_PC     ENB_STM
     8 TX                    28 T18
     9 GND        -          29 T16
    10 RX                    30 GND
    11 KA                    31 EN_DDA1
    12 T17                   32 ST_EXT    REQ_RD
    13 KB                    33 K_STOP    -
    14 GND                   34 GND
    15 K_START               35 EN_DDA2
    16 DT_DDAC  DAT_F0       36 FULL
    17                       37 FIT       -
    18 CLK_DDAC DAT_F1       38
    19          SPI1_MOSI    39 GND
    20 GND      GNDK         40          RDY


       Замены

       Было       Стало
    16 DAT_F0     DT_DDAC
    18 DAT_F1     CLK_DDAC
    22 DAT_F2     F_CON2
    26 SCICS      F_CON1
    27 ENB_STM    ENB_PC
    32 REQ_RD     ST_EXT
    33 -          KN_STOP
    37 -          FIT
    40 RDY        -

        Сделано
    19 SPI1_MOSI  -
    21 STOP       SPI_MISO
    23 SPI1_CLK   SPI_CLK
    24 DAT_F3     SPICS

    SPI0_M2:
    21 MISO
    23 CLK
    24 CS0
*/


class Pin
{
public:
    enum E
    {
        In_START,       // 15 Кнопка СТАРТ
        In_STOP,        // 21 Кнопка СТОП
        In_DAT_F0,      // 16
        In_SPI_MOSI,    // 18
        In_DAT_F2,      // 22
        Out_SPI_CS,     // 24
        In_FIFO_FULL,   // 36
        Out_REQ_RD,     // 32
        In_KA,          // 11 GPIO1_A4
        In_KB,          // 13 GPIO1_A7
        Out,            // 26 GPIO1_B5
        Count
    };
    explicit Pin(E type) : type_(type) { }

    E type() const
    {
        return type_;
    }

    bool Get() const;

protected:
    E type_;
};


class PinIn : public Pin
{
public:

    using Pin::Pin;

    using ChangeCallback = std::function<void(bool)>;

    void SetChangeCallback(ChangeCallback);

    // Время чтения пина - 0.5 мкс
    static bool GetHardware(gpiod_line *);

private:

    ChangeCallback callback_;
};


class PinOut : public Pin
{
public:

    using Pin::Pin;

    void Set(bool state);

    void ToLow();

    void ToHi();

    // Время установки пина - 0.5 мкс
    static void Set(gpiod_line *, int);
};

namespace GPIO
{
    void Init();
    void DeInit();

    InputPinInfo *GetInputPinInfo(Pin::E);
    OutputPinInfo *GetOutputPinInfo(Pin::E);
}


extern PinIn pinDAT_F0;
extern PinIn pinDAT_F2;
extern PinIn pinFIFO_FULL;

extern PinIn pinSTART;
extern PinIn pinSTOP;
extern PinIn pinKA;
extern PinIn pinKB;

extern PinOut pinREQ_RD;
extern PinOut pinOut;
